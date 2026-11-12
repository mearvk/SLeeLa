import java.beans.IndexedPropertyChangeEvent;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.beans.PropertyChangeListenerProxy;
import java.beans.PropertyChangeSupport;
import java.beans.PropertyVetoException;
import java.beans.VetoableChangeListener;
import java.beans.VetoableChangeSupport;

/** Java side of the BeanEvents differential test (java.beans bound and constrained properties, listeners as lambdas). */
public class BeanEvents {
    public static void main(String[] args) {
        String source = "bean";
        PropertyChangeSupport pcs = new PropertyChangeSupport(source);
        System.out.println(pcs.hasListeners("width") + " " + pcs.getPropertyChangeListeners().length);

        PropertyChangeListener all = e -> System.out.println("all: " + e.getPropertyName() + " " + e.getOldValue() + " -> " + e.getNewValue());
        String tag = "W";
        PropertyChangeListener width = e -> System.out.println(tag + ": " + e.getOldValue() + " -> " + e.getNewValue() + " from " + e.getSource());
        pcs.addPropertyChangeListener(all);
        pcs.addPropertyChangeListener("width", width);
        pcs.addPropertyChangeListener(null);
        System.out.println(pcs.hasListeners("width") + " " + pcs.hasListeners("height") + " " + pcs.getPropertyChangeListeners().length
            + " " + pcs.getPropertyChangeListeners("width").length + " " + pcs.getPropertyChangeListeners("height").length);
        PropertyChangeListener[] ls = pcs.getPropertyChangeListeners();
        System.out.println((ls[0] == all) + " " + (ls[1] instanceof PropertyChangeListenerProxy) + " " + ((PropertyChangeListenerProxy) ls[1]).getPropertyName()
            + " " + (((PropertyChangeListenerProxy) ls[1]).getListener() == width));

        pcs.firePropertyChange("width", 100, 200);
        pcs.firePropertyChange("width", 200, 200);
        pcs.firePropertyChange("height", null, "tall");
        pcs.firePropertyChange("title", "a", "a");
        pcs.firePropertyChange("title", null, null);
        pcs.firePropertyChange("flag", false, true);
        pcs.firePropertyChange(new PropertyChangeEvent(source, null, 1, 2));
        pcs.fireIndexedPropertyChange("width", 3, "x", "y");

        PropertyChangeEvent ev = new PropertyChangeEvent(source, "p", "o", "n");
        System.out.println(ev);
        ev.setPropagationId("pid");
        System.out.println(ev.getPropagationId() + " " + ev);
        IndexedPropertyChangeEvent iev = new IndexedPropertyChangeEvent(source, "cells", "a", "b", 7);
        System.out.println(iev.getIndex() + " " + iev.getPropertyName() + " " + iev);
        try { new PropertyChangeEvent(null, "p", 1, 2); } catch (IllegalArgumentException e) { System.out.println("IAE " + e.getMessage()); }

        pcs.removePropertyChangeListener(all);
        pcs.firePropertyChange("width", 1, 2);
        pcs.removePropertyChangeListener(new PropertyChangeListenerProxy("width", width));
        pcs.firePropertyChange("width", 2, 3);
        System.out.println(pcs.hasListeners("width") + " " + pcs.getPropertyChangeListeners().length);

        VetoableChangeSupport vcs = new VetoableChangeSupport(source);
        VetoableChangeListener log = e -> System.out.println("log: " + e.getPropertyName() + " " + e.getOldValue() + " -> " + e.getNewValue());
        VetoableChangeListener guard = e -> {
            System.out.println("guard: " + e.getOldValue() + " -> " + e.getNewValue());
            if ("forbidden".equals(e.getNewValue())) { throw new PropertyVetoException("no forbidden names", e); }
        };
        vcs.addVetoableChangeListener(log);
        vcs.addVetoableChangeListener("name", guard);
        try {
            vcs.fireVetoableChange("name", "alice", "bob");
            System.out.println("accepted bob");
            vcs.fireVetoableChange("name", "bob", "forbidden");
            System.out.println("accepted forbidden");
        } catch (PropertyVetoException v) {
            System.out.println("vetoed: " + v.getMessage() + " " + v.getPropertyChangeEvent().getNewValue());
        }
        try { vcs.fireVetoableChange("name", "same", "same"); System.out.println("no event for equal values"); }
        catch (PropertyVetoException v) { System.out.println("unexpected"); }
        System.out.println(vcs.hasListeners("name") + " " + vcs.getVetoableChangeListeners().length + " " + vcs.getVetoableChangeListeners("name").length);
        try { new PropertyChangeSupport(null); } catch (NullPointerException e) { System.out.println("NPE"); }
    }
}
