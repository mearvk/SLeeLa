;;; sleela-greek-theme.el --- SLeeLa Greek (Hellenic) theme -*- lexical-binding: t; -*-
;; The national colours of Greece. Source colours (Law 851/1978 on the National
;; Flag; Hellenic Republic MFA state symbols): national Blue #0D5EAF (the sky
;; and the seas) and White #FFFFFF (purity of the independence struggle; the
;; cross denotes Eastern Orthodox Christianity). Heraldic blazon: azure, four
;; bars argent. Interpretive dark design palette, not an official state
;; representation.
(deftheme sleela-greek "Hellenic national blue (#0D5EAF) and white, from the flag and state colours of Greece.")
(custom-theme-set-faces
 'sleela-greek
 '(default ((t (:foreground "#FFFFFF" :background "#0A1A30"))))
 '(cursor ((t (:background "#0D5EAF"))))
 '(region ((t (:background "#0D5EAF" :extend t))))
 '(highlight ((t (:background "#102744"))))
 '(fringe ((t (:foreground "#6E86A6" :background "#0A1A30"))))
 '(line-number ((t (:foreground "#6E86A6" :background "#0A1A30"))))
 '(line-number-current-line ((t (:foreground "#0D5EAF" :weight bold))))
 '(mode-line ((t (:foreground "#FFFFFF" :background "#0D5EAF" :weight bold))))
 '(mode-line-inactive ((t (:foreground "#6E86A6" :background "#102744"))))
 '(minibuffer-prompt ((t (:foreground "#2E7BD6" :weight bold))))
 '(font-lock-builtin-face ((t (:foreground "#5E9FE0"))))
 '(font-lock-comment-face ((t (:foreground "#6E86A6" :slant italic))))
 '(font-lock-constant-face ((t (:foreground "#4C8FDE"))))
 '(font-lock-function-name-face ((t (:foreground "#FFFFFF" :weight bold))))
 '(font-lock-keyword-face ((t (:foreground "#2E7BD6" :weight bold))))
 '(font-lock-string-face ((t (:foreground "#9FC3EC"))))
 '(font-lock-type-face ((t (:foreground "#5E9FE0" :weight bold))))
 '(font-lock-variable-name-face ((t (:foreground "#EAF1FB"))))
 '(font-lock-warning-face ((t (:foreground "#FF6B6B" :weight bold :underline t))))
 '(error ((t (:foreground "#FF6B6B" :weight bold))))
 '(warning ((t (:foreground "#9FC3EC" :weight bold))))
 '(success ((t (:foreground "#5E9FE0" :weight bold))))
 '(show-paren-match ((t (:foreground "#FFFFFF" :background "#0D5EAF" :weight bold))))
 '(isearch ((t (:foreground "#0A1A30" :background "#FFFFFF" :weight bold)))))
(provide-theme 'sleela-greek)
;;; sleela-greek-theme.el ends here
