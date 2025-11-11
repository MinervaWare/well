;; well-mode.el

(defconst well-mode-syntax-table
  (with-syntax-table (copy-syntax-table)
        (modify-syntax-entry ?/ ". 124b")
        (modify-syntax-entry ?* ". 23")
        (modify-syntax-entry ?\n "> b")
        (modify-syntax-entry ?' "\"")
        (syntax-table)
    )
  "Syntax table for 'well-mode'.")

(eval-and-compile
    (defconst well-keywords'(
        "push" "move" "lea" "call" "lib" "asm" "extern"
        "return" "syscall" "pop" "add" "sub" "int" "string"
        "char" "length" "func" "var")))

(defconst well-highlights
    `((,(regexp-opt well-keywords 'symbols) . font-lock-keyword-face)))

;;;###autoload
(define-derived-mode well-mode prog-mode "well"
    "Major Mode for editing Wellang source code."
    :syntax-table well-mode-syntax-table
    (setq font-lock-defaults '(well-highlights))
    (setq-local comment-start "// "))

;;;###autoload
(add-to-list 'auto-mode-alist '("\\.well\\'" . well-mode)

(provide 'well-mode')
