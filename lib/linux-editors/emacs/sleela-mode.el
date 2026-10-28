;;; sleela-mode.el --- Major mode for SLeeLa source files -*- lexical-binding: t; -*-
(require 'font-lock)
(defgroup sleela nil "Editing SLeeLa language source." :group 'languages)
(defconst sleela-font-lock-keywords
  (let ((keywords '("let" "var" "const" "if" "else" "while" "for" "in" "return" "break" "continue" "class" "interface" "extends" "implements" "new" "this" "super" "import" "package" "public" "private" "protected" "static" "final" "true" "false" "null" "nil" "and" "or" "not")))
    `(
      ;; Document pragmas: #sleela / #schema. Mapped to font-lock-builtin-face,
      ;; which every bundled sleela-*-theme defines (the themes do not style
      ;; font-lock-preprocessor-face), so pragmas/markers read distinctly from
      ;; ordinary keywords in all palettes.
      ("^[ \t]*#\\(?:sleela\\|schema\\)\\>.*$" . font-lock-builtin-face)
      ;; Document markers -- contextual words recognised only at document scope
      ;; (author/authorial = syntax 2.4; material = syntax 2.5). Anchored to a
      ;; line that is only the word and an optional ';', so an ordinary
      ;; identifier named e.g. 'material' elsewhere is NOT highlighted, mirroring
      ;; the compiler (SLEELA.syntax sections 18 and 19).
      ("^[ \t]*\\(authorial\\|author\\|material\\)[ \t]*;?[ \t]*$" 1 font-lock-builtin-face)
      (,(regexp-opt keywords 'words) . font-lock-keyword-face)
      ("\\_<\\(String\\|SLString\\|Integer\\|Int\\|Long\\|Float\\|Double\\|Boolean\\|Character\\|Byte\\|Short\\|Void\\|Object\\|List\\|Map\\|Set\\|Array\\)\\_>" . font-lock-type-face)
      ("@[A-Za-z_][A-Za-z0-9_]*" . font-lock-preprocessor-face)
      ;; The  (U+2234) originated-source brander (syntax 2.4), the house @.
      ("\u2234" . font-lock-constant-face)
      ("\\_<\\(TODO\\|FIXME\\|NOTE\\|XXX\\)\\_>" . font-lock-warning-face))))
(defvar sleela-mode-syntax-table
  (let ((table (make-syntax-table)))
    (modify-syntax-entry ?_ "w" table)
    ;; // line and /* */ block comments.
    (modify-syntax-entry ?/ ". 124b" table)
    (modify-syntax-entry ?* ". 23" table)
    (modify-syntax-entry 10 "> b" table)
    table))
;; The SLeeLa house-style angle comments ( << >> block and < > single-line )
;; are highlighted with font-lock regexps (the syntax table above keeps //, /*
;; */). An angle comment must begin a line (only whitespace before the opener).
(defconst sleela-angle-comment-keywords
  '(("^[ \t]*<<\\(?:.\\|\n\\)*?>>" 0 font-lock-comment-face t)
    ("^[ \t]*<[^<].*>?$" 0 font-lock-comment-face t)))
(defconst sleela-font-lock-keywords-all
  (append sleela-font-lock-keywords sleela-angle-comment-keywords))
;;;###autoload
(define-derived-mode sleela-mode prog-mode "SLeeLa"
  "Major mode for editing SLeeLa language source."
  :syntax-table sleela-mode-syntax-table
  (setq-local font-lock-defaults '(sleela-font-lock-keywords-all))
  (setq-local font-lock-multiline t)
  (setq-local comment-start "// ")
  (setq-local comment-end "")
  (setq-local indent-tabs-mode nil)
  (setq-local tab-width 4))
;;;###autoload
(add-to-list 'auto-mode-alist '("\\.sleela\\'" . sleela-mode))
(provide 'sleela-mode)
;;; sleela-mode.el ends here
