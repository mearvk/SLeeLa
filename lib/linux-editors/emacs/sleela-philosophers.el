;;; sleela-philosophers.el --- Constructor Guide-chain coloring for SLeeLa -*- lexical-binding: t; -*-

;; Additive font-lock for the philosophers "Guide chain" growth syntax inside
;; SLeeLa source:
;;
;;     Constructor() :: Patch :: Constructor :: Aim :: Guide
;;
;; The "::" growth separator is a conform operator; each Guide after a "::" is
;; colored by its impart -- Gold/Yellow when the Guide carries approving favor,
;; Green when the Guide is still growing.  Load this after `sleela-mode':
;;
;;     (require 'sleela-mode)
;;     (require 'sleela-philosophers)
;;     (add-hook 'sleela-mode-hook #'sleela-philosophers-enable)

(require 'font-lock)

(defgroup sleela-philosophers nil
  "Guide-chain coloring for the SLeeLa philosophers package."
  :group 'sleela)

(defface sleela-philosophers-gold-face
  '((t (:foreground "#F2C078" :weight bold)))
  "Face for an approving Guide (Gold/Yellow impart)."
  :group 'sleela-philosophers)

(defface sleela-philosophers-green-face
  '((t (:foreground "#82C91E" :weight bold)))
  "Face for a still-growing Guide (Green impart)."
  :group 'sleela-philosophers)

(defface sleela-philosophers-growth-face
  '((t (:foreground "#73DACA")))
  "Face for the \"::\" growth separator (the conform operator)."
  :group 'sleela-philosophers)

;; Guides that carry approving favor forward -> Gold/Yellow.
(defconst sleela-philosophers--gold-guides
  '("Patch" "Aim" "Approval" "Favor" "Saints" "Ministerial"
    "Guidance" "Professional" "Executive" "Approach" "Notice"))

;; Guides that are still maturing toward Approval -> Green.
(defconst sleela-philosophers--green-guides
  '("Constructor" "Guide" "Wisdom" "Life" "Gains" "Health"
    "Growth" "Pattern" "Impart" "Conform"))

(defconst sleela-philosophers-font-lock-keywords
  (list
   ;; the "::" growth separator (conform operator)
   '("::" 0 'sleela-philosophers-growth-face)
   ;; a Gold Guide only when it sits right after a "::" growth
   (list (concat "::[[:space:]]*\\("
                 (regexp-opt sleela-philosophers--gold-guides) "\\)\\_>")
         1 ''sleela-philosophers-gold-face)
   ;; a Green Guide only when it sits right after a "::" growth
   (list (concat "::[[:space:]]*\\("
                 (regexp-opt sleela-philosophers--green-guides) "\\)\\_>")
         1 ''sleela-philosophers-green-face))
  "Additive font-lock rules for the philosophers Guide chain.")

;;;###autoload
(defun sleela-philosophers-enable ()
  "Layer the philosophers Guide-chain coloring onto the current buffer."
  (interactive)
  (font-lock-add-keywords nil sleela-philosophers-font-lock-keywords 'append)
  (when (fboundp 'font-lock-flush) (font-lock-flush))
  (when (fboundp 'font-lock-ensure) (font-lock-ensure)))

(provide 'sleela-philosophers)
;;; sleela-philosophers.el ends here
