;; -*- coding: utf-8 -*-
;;(defvar best-gc-cons-threshold gc-cons-threshold "Best default gc threshold value. Should't be too big.")

;; Added by Package.el.  This must come before configurations of
;; installed packages.  Don't delete this line.  If you don't want it,
;; just comment it out by adding a semicolon to the start of the line.
;; You may delete these explanatory comments.
(package-initialize)

(defvar best-gc-cons-threshold 4000000 "Best default gc threshold value. Should't be too big.")
;; don't GC during startup to save time
(setq gc-cons-threshold most-positive-fixnum)

(setq emacs-load-start-time (current-time))
(add-to-list 'load-path (expand-file-name "~/.emacs.d/lisp"))

;;----------------------------------------------------------------------------
;; Which functionality to enable (use t or nil for true and false)
;;----------------------------------------------------------------------------
(setq *is-a-mac* (eq system-type 'darwin))
(setq *win64* (eq system-type 'windows-nt) )
(setq *cygwin* (eq system-type 'cygwin) )
(setq *linux* (or (eq system-type 'gnu/linux) (eq system-type 'linux)) )
(setq *unix* (or *linux* (eq system-type 'usg-unix-v) (eq system-type 'berkeley-unix)) )
(setq *emacs24* (and (not (featurep 'xemacs)) (or (>= emacs-major-version 24))) )
(setq *no-memory* (cond
                   (*is-a-mac*
                    (< (string-to-number (nth 1 (split-string (shell-command-to-string "sysctl hw.physmem")))) 4000000000))
                   (*linux* nil)
                   (t nil)))

(setq *emacs24old*  (or (and (= emacs-major-version 24) (= emacs-minor-version 3))
                        (not *emacs24*)))

;; *Message* buffer should be writable in 24.4+
(defadvice switch-to-buffer (after switch-to-buffer-after-hack activate)
  (if (string= "*Messages*" (buffer-name))
      (read-only-mode -1)))

;; start custom
(menu-bar-mode -1)
(blink-cursor-mode -1)
(setq gnus-inhibit-startup-message t)
;;不要临时文件
(setq make-backup-files nil)
(setq auto-save-default nil)
;;不产生备份
(setq backup-inhibited t)
(setq idle-require-message-verbose nil)

(setq user-full-name "nson7")
(setq user-mail-address "nson7@sina.com")

;;tab键为8个字符宽度
(setq default-tab-width 8)
(setq-default indent-tabs-mode nil)
(setq tab-stop-list ())
(setq c-default-style "Linux")
(setq c-basic-offset 4)

;;打开括号匹配显示模式
;; (show-paren-mode t)
;;括号匹配时可以高亮显示另外一边的括号，但光标不会烦人的跳到另一个括号处。
;; (setq show-paren-style 'parenthesis)

;; (set-default-font "Courier New-14")

;; (add-to-list 'load-path "~/.emacs.d/solarized")
;; (require 'color-theme-solarized)
;; (color-theme-solarized)

;; (add-to-list 'load-path "~/.emacs.d/molokai")
;; (require 'color-theme-molokai)
;; (color-theme-molokai)

;;设置打开文件的缺省路径
;; (setq default-directory "~/.emacs.d")

;;关闭emacs启动时的画面
;; (setq inhibit-startup-message t)

;; (fset 'yes-or-no-p 'y-or-n-p)
;; 改变 Emacs 固执的要你回答 yes 的行为。按 y 或空格键表示 yes，n 表示 no。

;;显示行列号
;; (global-linum-mode t)
;; (setq column-number-mode t)
;; (setq line-number-mode t)

;; (setq auto-image-file-mode t)
;;让 Emacs 可以直接打开和显示图片。

;(auto-compression-mode 1)
;打开压缩文件时自动解压缩。

;;[ 高亮当前行 ]
;;----------------------------------------------------------------------------
;; (require 'hl-line)
;; (global-hl-line-mode t)

;; end custom

;; @see https://www.reddit.com/r/emacs/comments/3kqt6e/2_easy_little_known_steps_to_speed_up_emacs_start/
;; Normally file-name-handler-alist is set to
;; (("\\`/[^/]*\\'" . tramp-completion-file-name-handler)
;; ("\\`/[^/|:][^/|]*:" . tramp-file-name-handler)
;; ("\\`/:" . file-name-non-special))
;; Which means on every .el and .elc file loaded during start up, it has to runs those regexps against the filename.
(let ((file-name-handler-alist nil))
  (require 'init-autoload)
  (require 'init-modeline)
  (require 'cl-lib)
  (require 'init-compat)
  (require 'init-utils)
  (require 'init-site-lisp) ;; Must come before elpa, as it may provide package.el

  ;; Windows configuration, assuming that cygwin is installed at "c:/cygwin"
  ;; (condition-case nil
  ;;     (when *win64*
  ;;       ;; (setq cygwin-mount-cygwin-bin-directory "c:/cygwin/bin")
  ;;       (setq cygwin-mount-cygwin-bin-directory "c:/cygwin64/bin")
  ;;       (require 'setup-cygwin)
  ;;       ;; better to set HOME env in GUI
  ;;       ;; (setenv "HOME" "c:/cygwin/home/someuser")
  ;;       )
  ;;   (error
  ;;    (message "setup-cygwin failed, continue anyway")
  ;;    ))

  (require 'idle-require)
  (require 'init-elpa)
  (require 'init-exec-path) ;; Set up $PATH
  ;; any file use flyspell should be initialized after init-spelling.el
  ;; actually, I don't know which major-mode use flyspell.
  (require 'init-spelling)
  (require 'init-gui-frames)
  (require 'init-ido)
  (require 'init-dired)
  (require 'init-uniquify)
  (require 'init-ibuffer)
  (require 'init-ivy)
  (require 'init-hippie-expand)
  (require 'init-windows)
  (require 'init-sessions)
  (require 'init-git)
  (require 'init-crontab)
  (require 'init-markdown)
  (require 'init-erlang)
  (require 'init-javascript)
  (require 'init-org)
  (require 'init-css)
  (require 'init-python-mode)
  (require 'init-haskell)
  (require 'init-ruby-mode)
  (require 'init-lisp)
  (require 'init-elisp)
  (require 'init-yasnippet)
  ;; Use bookmark instead
  (require 'init-cc-mode)
  (require 'init-gud)
  (require 'init-linum-mode)
  ;; (require 'init-gist)
  (require 'init-moz)
  (require 'init-gtags)
  ;; init-evil dependent on init-clipboard
  (require 'init-clipboard)
  ;; use evil mode (vi key binding)
  (require 'init-evil)
  (require 'init-multiple-cursors)
  (require 'init-sh)
  (require 'init-ctags)
  (require 'init-bbdb)
  (require 'init-gnus)
  (require 'init-lua-mode)
  (require 'init-workgroups2)
  (require 'init-term-mode)
  (require 'init-web-mode)
  (require 'init-slime)
  (require 'init-company)
  (require 'init-chinese-pyim) ;; cannot be idle-required
  ;; need statistics of keyfreq asap
  (require 'init-keyfreq)
  (require 'init-httpd)

  ;; projectile costs 7% startup time

  ;; misc has some crucial tools I need immediately
  (require 'init-misc)

  ;; comment below line if you want to setup color theme in your own way
  (if (or (display-graphic-p) (string-match-p "256color"(getenv "TERM"))) (require 'init-color-theme))

  (require 'init-emacs-w3m)
  (require 'init-hydra)

  ;; {{ idle require other stuff
  (setq idle-require-idle-delay 2)
  (setq idle-require-symbols '(init-misc-lazy
                               init-which-func
                               init-fonts
                               init-hs-minor-mode
                               init-writting
                               init-pomodoro
                               init-emacspeak
                               init-artbollocks-mode
                               init-semantic))
  (idle-require-mode 1) ;; starts loading
  ;; }}

  (when (require 'time-date nil t)
    (message "Emacs startup time: %d seconds."
             (time-to-seconds (time-since emacs-load-start-time))))

  ;; my personal setup, other major-mode specific setup need it.
  ;; It's dependent on init-site-lisp.el
  (if (file-exists-p "~/.custom.el") (load-file "~/.custom.el")))

;; start custom
(add-to-list 'load-path "~/.emacs.d/site-lisp/find-and-ctags")
(require 'find-and-ctags)

(defun my-setup-develop-environment ()
  (interactive)
  (let (proj-dir
        find-opts
        ctags-opts)

    ;; for COOL MYPROJ
    ;; you can use find-and-ctags-current-full-filename-match-pattern-p instead
    (when (find-and-ctags-current-path-match-pattern-p "/root")
      (setq proj-dir (if find-and-ctags-windows-p "c:/project/app"
                       "~/"))
      ;; ignore file bigger than 64K, ignore files in "dist/"
      (setq find-opts "-not -size +64k -not -iwholename '*/dist/*'")
      (setq ctags-opts "--exclude='*.min.js' --exclude='*.git*'")
      ;; you can use setq-local instead
      (setq tags-table-list
            (list (find-and-ctags-run-ctags-if-needed proj-dir '((find-opts ctags-opts)
                                                                 ("dist/test.js" "-a"))))))
    ;; for other projects
    ;; insert more `when' statements here
    ))

;; OPTIONAL
(add-hook 'after-save-hook 'find-and-ctags-auto-update-tags)
(add-hook 'prog-mode-hook 'my-setup-develop-environment)
(add-hook 'org-mode-hook 'my-setup-develop-environment)

;; In above setup, TAGS will be updated *automatically* every 5 minutes.
;; But you can manually update TAGS by `M-x find-and-ctags-update-all-tags-force'.
;; If you want to manually update the TAGS, `M-x find-and-ctags-update-all-tags-force'.
;;
;; After `'tags-table-list' is set, You can `M-x find-tag' to start code navigation
;;
;; You can use `(find-and-ctags-get-hostname)' for per computer setup.
;; For example, if my home PC hostname is like `AU0247589',
;; Here is sample code how I specify my C++ setup for home ONLY:
;;
;;   (if (string-match "^AU.*" (find-and-ctags-get-hostname))
;;      (setq my-default-ctags-options "--I IMPLEMENT_ABSTRACT_CLASS"))
;;
;; See https://github.com/redguardtoo/find-and-ctags/ for more tips

;;; Code:

(autoload 'find-file-in-project "find-file-in-project" nil t)
(autoload 'find-file-in-project-by-selected "find-file-in-project" nil t)
(autoload 'find-directory-in-project-by-selected "find-file-in-project" nil t)
(autoload 'ffip-show-diff "find-file-in-project" nil t)
(autoload 'ffip-save-ivy-last "find-file-in-project" nil t)
(autoload 'ffip-ivy-resume "find-file-in-project" nil t)

;; end custom

;; @see https://www.reddit.com/r/emacs/comments/4q4ixw/how_to_forbid_emacs_to_touch_configuration_files/
(setq custom-file (concat user-emacs-directory "custom-set-variables.el"))
(load custom-file 'noerror)

(setq gc-cons-threshold best-gc-cons-threshold)
;;; Local Variables:
;;; no-byte-compile: t
;;; End:
(put 'erase-buffer 'disabled nil)
