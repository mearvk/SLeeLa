;;; sleela-turkish-theme.el --- SLeeLa Turkish (Al Bayrak) theme -*- lexical-binding: t; -*-
;; Derived from the national colours of Turkiye (Turkish Flag Law): Red #E30A17
;; and White #FFFFFF. Interpretive dark design palette, not an official
;; representation of the Republic of Turkiye.
(deftheme sleela-turkish "Al Bayrak crimson red and crescent white palette, from the flag of Turkiye.")
(custom-theme-set-faces
 'sleela-turkish
 '(default ((t (:foreground "#F6EEEF" :background "#2A0A0D"))))
 '(cursor ((t (:background "#E8505B"))))
 '(region ((t (:background "#8A1A22" :extend t))))
 '(highlight ((t (:background "#3C1115"))))
 '(fringe ((t (:foreground "#A8767A" :background "#2A0A0D"))))
 '(line-number ((t (:foreground "#A8767A" :background "#2A0A0D"))))
 '(line-number-current-line ((t (:foreground "#E8505B" :weight bold))))
 '(mode-line ((t (:foreground "#FFFFFF" :background "#E30A17" :weight bold))))
 '(mode-line-inactive ((t (:foreground "#A8767A" :background "#3C1115"))))
 '(minibuffer-prompt ((t (:foreground "#E8505B" :weight bold))))
 '(font-lock-builtin-face ((t (:foreground "#F0A9AE"))))
 '(font-lock-comment-face ((t (:foreground "#A8767A" :slant italic))))
 '(font-lock-constant-face ((t (:foreground "#E87A82"))))
 '(font-lock-function-name-face ((t (:foreground "#F6E3E4" :weight bold))))
 '(font-lock-keyword-face ((t (:foreground "#E8505B" :weight bold))))
 '(font-lock-string-face ((t (:foreground "#F0C9A9"))))
 '(font-lock-type-face ((t (:foreground "#F0A9AE" :weight bold))))
 '(font-lock-variable-name-face ((t (:foreground "#F6EEEF"))))
 '(font-lock-warning-face ((t (:foreground "#FF8A3D" :weight bold :underline t))))
 '(error ((t (:foreground "#FF8A3D" :weight bold))))
 '(warning ((t (:foreground "#F0C9A9" :weight bold))))
 '(success ((t (:foreground "#F0A9AE" :weight bold))))
 '(show-paren-match ((t (:foreground "#FFFFFF" :background "#8A1A22" :weight bold))))
 '(isearch ((t (:foreground "#2A0A0D" :background "#F0C9A9" :weight bold)))))
(provide-theme 'sleela-turkish)
;;; sleela-turkish-theme.el ends here
