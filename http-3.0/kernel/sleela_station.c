// SPDX-License-Identifier: GPL-2.0
/* ==========================================================================
 * sleela_station.c -- SLeeLa HTTP 3.0 Station kernel driver (J-STA-0001 §9).
 *
 * A Router-side Linux kernel module that lets a Router act as an International
 * Station: it WITNESSES a two-party TCP/IP control-flag negotiation and stores
 * the classified record. It provides:
 *
 *   - a character device /dev/sleela_station with the ioctl API in station_abi.h
 *     (OPEN / SIGNAL / QUERY / CLOSE), the "known API" for direct calls;
 *   - a Netfilter hook that inspects TCP control flags on signaling segments,
 *     builds an sl_signal_t, and folds it into the matching session using the
 *     SAME classifier/session logic as userland (station_classifier.c /
 *     station_session.c, included below so the judgment is identical).
 *
 * This module observes and records. It does not modify or drop ordinary
 * traffic; it only witnesses signaling segments addressed to the Station's
 * signaling port.
 *
 * NOTE: requires kernel build headers (/lib/modules/$(uname -r)/build) to
 * compile; see Kbuild/Makefile in this directory. It is written to be built
 * out-of-tree on a host that has them.
 * ========================================================================== */
#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/mutex.h>
#include <linux/list.h>
#include <linux/ktime.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/skbuff.h>
#include <linux/atomic.h>

#include "../station/station_abi.h"
#include "../station/station_classifier.h"
#include "../station/station_session.h"

/* The shared Station logic is compiled straight into the module so a Router's
 * in-kernel judgment is byte-identical to userland's. */
#include "../station/station_classifier.c"
#include "../station/station_session.c"

#define SL_STATION_SIGNAL_PORT 9444  /* TCP dport that carries signaling */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("MEARVK LLC");
MODULE_DESCRIPTION("SLeeLa HTTP 3.0 Station: witness a TCP-flag negotiation (J-STA-0001)");
MODULE_VERSION("1.0.0");

/* ---- Session registry ----------------------------------------------------- */

struct sl_session_node {
    struct list_head list;
    sl_session_t     session;
};

static LIST_HEAD(sl_sessions);
static DEFINE_MUTEX(sl_lock);
static atomic64_t sl_next_session_id = ATOMIC64_INIT(1);

static struct sl_session_node *find_session_locked(u64 id)
{
    struct sl_session_node *n;
    list_for_each_entry(n, &sl_sessions, list) {
        if (n->session.session_id == id)
            return n;
    }
    return NULL;
}

static u64 now_ns(void)
{
    return (u64)ktime_get_real_ns();
}

/* ---- Character device: the known ioctl API -------------------------------- */

static int sl_dev_open(struct inode *inode, struct file *filp)
{
    (void)inode; (void)filp;
    return 0;
}

static int sl_dev_release(struct inode *inode, struct file *filp)
{
    (void)inode; (void)filp;
    return 0;
}

static long sl_do_open(void __user *arg)
{
    struct sl_session_node *node;
    sl_session_view_t view;
    u64 id;

    node = kzalloc(sizeof(*node), GFP_KERNEL);
    if (!node)
        return -ENOMEM;

    id = (u64)atomic64_inc_return(&sl_next_session_id);
    sl_session_open(&node->session, id, now_ns());

    mutex_lock(&sl_lock);
    list_add_tail(&node->list, &sl_sessions);
    sl_session_view(&node->session, &view);
    mutex_unlock(&sl_lock);

    if (copy_to_user(arg, &view, sizeof(view)))
        return -EFAULT;
    return 0;
}

static long sl_do_signal(void __user *arg)
{
    sl_signal_t sig;
    sl_record_t rec;
    struct sl_session_node *node;
    int rc;

    if (copy_from_user(&sig, arg, sizeof(sig)))
        return -EFAULT;
    if (sig.abi_version != SLEELA_STATION_ABI_VERSION)
        return -EINVAL;

    mutex_lock(&sl_lock);
    node = find_session_locked(sig.session_id);
    if (!node) {
        mutex_unlock(&sl_lock);
        return -ENOENT;
    }
    rc = sl_session_signal(&node->session, &sig, &rec);
    mutex_unlock(&sl_lock);
    if (rc != 0)
        return -EINVAL;

    if (copy_to_user(arg, &sig, sizeof(sig))) /* echo input; record via QUERY */
        return -EFAULT;
    return 0;
}

static long sl_do_query(void __user *arg)
{
    sl_session_view_t view;
    struct sl_session_node *node;

    if (copy_from_user(&view, arg, sizeof(view)))
        return -EFAULT;

    mutex_lock(&sl_lock);
    node = find_session_locked(view.session_id);
    if (!node) {
        mutex_unlock(&sl_lock);
        return -ENOENT;
    }
    sl_session_view(&node->session, &view);
    mutex_unlock(&sl_lock);

    if (copy_to_user(arg, &view, sizeof(view)))
        return -EFAULT;
    return 0;
}

static long sl_do_close(void __user *arg)
{
    struct sl_session_node *node;
    u64 id;

    if (copy_from_user(&id, arg, sizeof(id)))
        return -EFAULT;

    mutex_lock(&sl_lock);
    node = find_session_locked(id);
    if (node)
        sl_session_close(&node->session, now_ns());
    mutex_unlock(&sl_lock);
    return node ? 0 : -ENOENT;
}

static long sl_dev_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    void __user *uarg = (void __user *)arg;
    (void)filp;
    switch (cmd) {
    case SL_IOC_OPEN:   return sl_do_open(uarg);
    case SL_IOC_SIGNAL: return sl_do_signal(uarg);
    case SL_IOC_QUERY:  return sl_do_query(uarg);
    case SL_IOC_CLOSE:  return sl_do_close(uarg);
    default:            return -ENOTTY;
    }
}

static const struct file_operations sl_fops = {
    .owner          = THIS_MODULE,
    .open           = sl_dev_open,
    .release        = sl_dev_release,
    .unlocked_ioctl = sl_dev_ioctl,
    .compat_ioctl   = sl_dev_ioctl,
};

static dev_t sl_devno;
static struct cdev sl_cdev;
static struct class *sl_class;

/* ---- Netfilter hook: parse TCP flags on signaling segments ---------------- */

/*
 * The URGENT count and hop delta are, in a deployment, carried in negotiated
 * header fields of the signaling segment. Here we derive urg_units from the TCP
 * urgent pointer (clamped to 0..8) and hop_delta from the low 2 bits of the IP
 * TTL relative to the Station's configured DIST (module reads TTL as a stand-in
 * for the negotiated hop field). This keeps the kernel path honest about where
 * the values come from while the classifier stays identical to userland.
 */
static unsigned int sl_nf_hook(void *priv, struct sk_buff *skb,
                               const struct nf_hook_state *state)
{
    struct iphdr *iph;
    struct tcphdr *tcph;
    sl_signal_t sig;
    struct sl_session_node *node;

    (void)priv; (void)state;
    if (!skb)
        return NF_ACCEPT;
    iph = ip_hdr(skb);
    if (!iph || iph->protocol != IPPROTO_TCP)
        return NF_ACCEPT;
    tcph = tcp_hdr(skb);
    if (!tcph)
        return NF_ACCEPT;
    if (ntohs(tcph->dest) != SL_STATION_SIGNAL_PORT)
        return NF_ACCEPT; /* only witness signaling-port segments */

    memset(&sig, 0, sizeof(sig));
    sig.abi_version = SLEELA_STATION_ABI_VERSION;
    /* Sessions are keyed by source port here as a simple demonstration key. */
    sig.session_id = (u64)ntohs(tcph->source);
    sig.party = (ntohs(tcph->source) & 1u) ? SL_PARTY_B : SL_PARTY_A;
    sig.tcp_flags = 0;
    if (tcph->fin) sig.tcp_flags |= SL_TCP_FIN;
    if (tcph->syn) sig.tcp_flags |= SL_TCP_SYN;
    if (tcph->rst) sig.tcp_flags |= SL_TCP_RST;
    if (tcph->psh) sig.tcp_flags |= SL_TCP_PSH;
    if (tcph->ack) sig.tcp_flags |= SL_TCP_ACK;
    if (tcph->urg) sig.tcp_flags |= SL_TCP_URG;
    {
        u16 uptr = ntohs(tcph->urg_ptr);
        sig.urg_units = (uint8_t)(uptr > SL_STATION_URG_MAX ? SL_STATION_URG_MAX : uptr);
    }
    /* hop_delta stand-in: (TTL & 3) - 1 maps to {-1,0,1,2}->clamped later. */
    sig.hop_delta = (int8_t)((int)(iph->ttl & 0x3) - 1);
    sig.timestamp_ns = now_ns();

    mutex_lock(&sl_lock);
    node = find_session_locked(sig.session_id);
    if (node)
        (void)sl_session_signal(&node->session, &sig, NULL);
    mutex_unlock(&sl_lock);

    return NF_ACCEPT; /* witness only; never drop */
}

static struct nf_hook_ops sl_nf_ops = {
    .hook     = sl_nf_hook,
    .pf       = NFPROTO_IPV4,
    .hooknum  = NF_INET_PRE_ROUTING,
    .priority = NF_IP_PRI_FIRST,
};

/* ---- Module lifecycle ----------------------------------------------------- */

static int __init sl_station_init(void)
{
    int rc;

    rc = alloc_chrdev_region(&sl_devno, 0, 1, SLEELA_STATION_DEVICE);
    if (rc)
        return rc;

    cdev_init(&sl_cdev, &sl_fops);
    sl_cdev.owner = THIS_MODULE;
    rc = cdev_add(&sl_cdev, sl_devno, 1);
    if (rc)
        goto err_region;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 4, 0)
    sl_class = class_create(SLEELA_STATION_DEVICE);
#else
    sl_class = class_create(THIS_MODULE, SLEELA_STATION_DEVICE);
#endif
    if (IS_ERR(sl_class)) {
        rc = PTR_ERR(sl_class);
        goto err_cdev;
    }
    if (IS_ERR(device_create(sl_class, NULL, sl_devno, NULL, SLEELA_STATION_DEVICE))) {
        rc = -ENODEV;
        goto err_class;
    }

    rc = nf_register_net_hook(&init_net, &sl_nf_ops);
    if (rc)
        goto err_device;

    pr_info("sleela_station: Station online (dev=%s, signal port=%d)\n",
            SLEELA_STATION_DEVPATH, SL_STATION_SIGNAL_PORT);
    return 0;

err_device:
    device_destroy(sl_class, sl_devno);
err_class:
    class_destroy(sl_class);
err_cdev:
    cdev_del(&sl_cdev);
err_region:
    unregister_chrdev_region(sl_devno, 1);
    return rc;
}

static void __exit sl_station_exit(void)
{
    struct sl_session_node *n, *tmp;

    nf_unregister_net_hook(&init_net, &sl_nf_ops);
    device_destroy(sl_class, sl_devno);
    class_destroy(sl_class);
    cdev_del(&sl_cdev);
    unregister_chrdev_region(sl_devno, 1);

    mutex_lock(&sl_lock);
    list_for_each_entry_safe(n, tmp, &sl_sessions, list) {
        list_del(&n->list);
        kfree(n);
    }
    mutex_unlock(&sl_lock);

    pr_info("sleela_station: Station offline\n");
}

module_init(sl_station_init);
module_exit(sl_station_exit);
