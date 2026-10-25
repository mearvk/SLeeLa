;;; sleela-turkish-theme.el --- SLeeLa Turkish (Al Bayrak) theme -*- lexical-binding: t; -*-
;; The national colours of Turkiye. Source colours (Turkish Flag Law; the flag
;; is al bayrak, the red flag): national Red #E30A17 (the field; by tradition
;; the blood of martyrs and Ottoman sovereignty) and White #FFFFFF (the crescent
;; and star -- peace, purity, and the Turkic/Islamic heritage symbol).
;; Interpretive dark design palette, not an official state representation.
(deftheme sleela-turkish "Al Bayrak national red (#E30A17) and crescent white, from the flag and state colours of Turkiye.")
(custom-theme-set-faces
 'sleela-turkish
 '(default ((t (:foreground "#FFFFFF" :background "#2A0609"))))
 '(cursor ((t (:background "#E30A17"))))
 '(region ((t (:background "#E30A17" :extend t))))
 '(highlight ((t (:background "#3E0F13"))))
 '(fringe ((t (:foreground "#A8767A" :background "#2A0609"))))
 '(line-number ((t (:foreground "#A8767A" :background "#2A0609"))))
 '(line-number-current-line ((t (:foreground "#E30A17" :weight bold))))
 '(mode-line ((t (:foreground "#FFFFFF" :background "#E30A17" :weight bold))))
 '(mode-line-inactive ((t (:foreground "#A8767A" :background "#3E0F13"))))
 '(minibuffer-prompt ((t (:foreground "#F23D48" :weight bold))))
 '(font-lock-builtin-face ((t (:foreground "#F0A9AE"))))
 '(font-lock-comment-face ((t (:foreground "#A8767A" :slant italic))))
 '(font-lock-constant-face ((t (:foreground "#E8505B"))))
 '(font-lock-function-name-face ((t (:foreground "#FFFFFF" :weight bold))))
 '(font-lock-keyword-face ((t (:foreground "#F23D48" :weight bold))))
 '(font-lock-string-face ((t (:foreground "#F0C9A9"))))
 '(font-lock-type-face ((t (:foreground "#F0A9AE" :weight bold))))
 '(font-lock-variable-name-face ((t (:foreground "#FBEAEB"))))
 '(font-lock-warning-face ((t (:foreground "#FF8A3D" :weight bold :underline t))))
 '(error ((t (:foreground "#FF8A3D" :weight bold))))
 '(warning ((t (:foreground "#F0C9A9" :weight bold))))
 '(success ((t (:foreground "#F0A9AE" :weight bold))))
 '(show-paren-match ((t (:foreground "#FFFFFF" :background "#E30A17" :weight bold))))
 '(isearch ((t (:foreground "#2A0609" :background "#FFFFFF" :weight bold)))))
(provide-theme 'sleela-turkish)
;;; sleela-turkish-theme.el ends here
