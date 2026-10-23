;;; sleela-mode.el --- Major mode for SLeeLa source files -*- lexical-binding: t; -*-
(require 'font-lock)
(defgroup sleela nil "Editing SLeeLa language source." :group 'languages)
(defconst sleela-font-lock-keywords
  (let ((keywords '("let" "var" "const" "if" "else" "while" "for" "in" "return" "break" "continue" "class" "interface" "extends" "implements" "new" "this" "super" "import" "package" "public" "private" "protected" "static" "final" "true" "false" "null" "nil" "and" "or" "not")))
    `((,(regexp-opt keywords 'words) . font-lock-keyword-face)
      ("\\_<\\(String\\|SLString\\|Integer\\|Int\\|Long\\|Float\\|Double\\|Boolean\\|Character\\|Byte\\|Short\\|Void\\|Object\\|List\\|Map\\|Set\\|Array\\)\\_>" . font-lock-type-face)
      ("@[A-Za-z_][A-Za-z0-9_]*" . font-lock-preprocessor-face)
      ("\\_<\\(TODO\\|FIXME\\|NOTE\\|XXX\\)\\_>" . font-lock-warning-face))))
(defvar sleela-mode-syntax-table
  (let ((table (make-syntax-table)))
    (modify-syntax-entry ?_ "w" table)
    (modify-syntax-entry ?/ ". 124b" table)
    (modify-syntax-entry ?* ". 23" table)
    (modify-syntax-entry 10 "> b" table)
    table))
;;;###autoload
(define-derived-mode sleela-mode prog-mode "SLeeLa"
  "Major mode for editing SLeeLa language source."
  :syntax-table sleela-mode-syntax-table
  (setq-local font-lock-defaults '(sleela-font-lock-keywords))
  (setq-local comment-start "// ")
  (setq-local comment-end "")
  (setq-local indent-tabs-mode nil)
  (setq-local tab-width 4))
;;;###autoload
(add-to-list 'auto-mode-alist '("\\.sleela\\'" . sleela-mode))
(provide 'sleela-mode)
;;; sleela-mode.el ends here
