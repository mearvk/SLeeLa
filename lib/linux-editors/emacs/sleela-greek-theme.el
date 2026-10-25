;;; sleela-greek-theme.el --- SLeeLa Greek (Hellenic) theme -*- lexical-binding: t; -*-
;; Derived from the national colours of Greece (Law 851/1978): Blue #0D5EAF and
;; White #FFFFFF. Interpretive dark design palette, not an official
;; representation of the Hellenic Republic.
(deftheme sleela-greek "Hellenic sea blue and marble white palette, from the flag of Greece.")
(custom-theme-set-faces
 'sleela-greek
 '(default ((t (:foreground "#EEF3FA" :background "#0B1B33"))))
 '(cursor ((t (:background "#4C8FDE"))))
 '(region ((t (:background "#1C4C86" :extend t))))
 '(highlight ((t (:background "#122744"))))
 '(fringe ((t (:foreground "#6E86A6" :background "#0B1B33"))))
 '(line-number ((t (:foreground "#6E86A6" :background "#0B1B33"))))
 '(line-number-current-line ((t (:foreground "#4C8FDE" :weight bold))))
 '(mode-line ((t (:foreground "#FFFFFF" :background "#0D5EAF" :weight bold))))
 '(mode-line-inactive ((t (:foreground "#6E86A6" :background "#122744"))))
 '(minibuffer-prompt ((t (:foreground "#4C8FDE" :weight bold))))
 '(font-lock-builtin-face ((t (:foreground "#7FB2E8"))))
 '(font-lock-comment-face ((t (:foreground "#6E86A6" :slant italic))))
 '(font-lock-constant-face ((t (:foreground "#6FA8E0"))))
 '(font-lock-function-name-face ((t (:foreground "#E8EEF6" :weight bold))))
 '(font-lock-keyword-face ((t (:foreground "#4C8FDE" :weight bold))))
 '(font-lock-string-face ((t (:foreground "#9FC3EC"))))
 '(font-lock-type-face ((t (:foreground "#7FB2E8" :weight bold))))
 '(font-lock-variable-name-face ((t (:foreground "#EEF3FA"))))
 '(font-lock-warning-face ((t (:foreground "#FF6B6B" :weight bold :underline t))))
 '(error ((t (:foreground "#FF6B6B" :weight bold))))
 '(warning ((t (:foreground "#9FC3EC" :weight bold))))
 '(success ((t (:foreground "#7FB2E8" :weight bold))))
 '(show-paren-match ((t (:foreground "#FFFFFF" :background "#1C4C86" :weight bold))))
 '(isearch ((t (:foreground "#0B1B33" :background "#9FC3EC" :weight bold)))))
(provide-theme 'sleela-greek)
;;; sleela-greek-theme.el ends here
