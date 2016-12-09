*cmdline.txt*   For Vim version 7.4.  Last change: 2015 Jul 28


		  VIM REFERENCE MANUAL    by Bram Moolenaar


				*Cmdline-mode* *Command-line-mode*
Command-line mode		*Cmdline* *Command-line* *mode-cmdline* *:*

Command-line mode is used to enter Ex commands (":"), search patterns
("/" and "?"), and filter commands ("!").

Basic command line editing is explained in chapter 20 of the user manual
|usr_20.txt|.

1. Command-line editing		|cmdline-editing|
2. Command-line completion	|cmdline-completion|
3. Ex command-lines		|cmdline-lines|
4. Ex command-line ranges	|cmdline-ranges|
5. Ex command-line flags	|ex-flags|
6. Ex special characters	|cmdline-special|
7. Command-line window		|cmdline-window|

==============================================================================
1. Command-line editing					*cmdline-editing*

Normally characters are inserted in front of the cursor position.  You can
move around in the command-line with the left and right cursor keys.  With the
<Insert> key, you can toggle between inserting and overstriking characters.
{Vi: can only alter the last character in the line}

Note that if your keyboard does not have working cursor keys or any of the
other special keys, you can use ":cnoremap" to define another key for them.
For example, to define tcsh style editing keys:		*tcsh-style*  >
	:cnoremap <C-A> <Home>
	:cnoremap <C-F> <Right>
	:cnoremap <C-B> <Left>
	:cnoremap <Esc>b <S-Left>
	:cnoremap <Esc>f <S-Right>
(<> notation |<>|; type all this literally)

							*cmdline-too-long*
When the command line is getting longer than what fits on the screen, only the
part that fits will be shown.  The cursor can only move in this visible part,
thus you cannot edit beyond that.

						*cmdline-history* *history*
The command-lines that you enter are remembered in a history table.  You can
recall them with the up and down cursor keys.  There are actually five
history tables:
- one for ':' commands
- one for search strings
- one for expressions
- one for input lines, typed for the |input()| function.
- one for debug mode commands
These are completely separate.  Each history can only be accessed when
entering the same type of line.
Use the 'history' option to set the number of lines that are remembered
(default: 50).
Notes:
- When you enter a command-line that is exactly the same as an older one, the
  old one is removed (to avoid repeated commands moving older commands out of
  the history).
- Only commands that are typed are remembered.  Ones that completely come from
  mappings are not put in the history.
- All searches are put in the search history, including the ones that come
  from commands like "*" and "#".  But for a mapping, only the last search is
  remembered (to avoid that long mappings trash the history).
{Vi: no history}
{not available when compiled without the |+cmdline_hist| feature}

There is an automatic completion of names on the command-line; see
|cmdline-completion|.

							*c_CTRL-V*
CTRL-V		Insert next non-digit literally.  Up to three digits form the
		decimal value of a single byte.  The non-digit and the three
		digits are not considered for mapping.  This works the same
		way as in Insert mode (see above, |i_CTRL-V|).
		Note: Under Windows CTRL-V is often mapped to paste text.
		Use CTRL-Q instead then.
							*c_CTRL-Q*
CTRL-Q		Same as CTRL-V.  But with some terminals it is used for
		control flow, it doesn't work then.

							*c_<Left>* *c_Left*
<Left>		cursor left
							*c_<Right>* *c_Right*
<Right>		cursor right
							*c_<S-Left>*
<S-Left> or <C-Left>					*c_<C-Left>*
		cursor one WORD left
							*c_<S-Right>*
<S-Right> or <C-Right>					*c_<C-Right>*
		cursor one WORD right
CTRL-B or <Home>				*c_CTRL-B* *c_<Home>* *c_Home*
		cursor to beginning of command-line
CTRL-E or <End>					*c_CTRL-E* *c_<End>* *c_End*
		cursor to end of command-line

							*c_<LeftMouse>*
<LeftMouse>	Move the cursor to the position of the mouse click.

CTRL-H						*c_<BS>* *c_CTRL-H* *c_BS*
<BS>		Delete the character in front of the cursor (see |:fixdel| if
		your <BS> key does not do what you want).
							*c_<Del>* *c_Del*
<Del>		Delete the character under the cursor (at end of line:
		character before the cursor) (see |:fixdel| if your <Del>
		key does not do what you want).
							*c_CTRL-W*
CTRL-W		Delete the |word| before the cursor.  This depends on the
		'iskeyword' option.
							*c_CTRL-U*
CTRL-U		Remove all characters between the cursor position and
		the beginning of the line.  Previous versions of vim
		deleted all characters on the line.  If that is the
		preferred behavior, add the following to your .vimrc: >
			:cnoremap <C-U> <C-E><C-U>
<
						*c_<Insert>* *c_Insert*
<Insert>	Toggle between insert and overstrike.  {not in Vi}

{char1} <BS> {char2}	or				*c_digraph*
CTRL-K {char1} {char2}					*c_CTRL-K*
		enter digraph (see |digraphs|).  When {char1} is a special
		key, the code for that key is inserted in <> form.  {not in Vi}

CTRL-R {0-9a-z"%#:-=.}					*c_CTRL-R* *c_<C-R>*
		Insert the contents of a numbered or named register.  Between
		typing CTRL-R and the second character '"' will be displayed
		to indicate that you are expected to enter the name of a
		register.
		The text is inserted as if you typed it, but mappings and
		abbreviations are not used.  Command-line completion through
		'wildchar' is not triggered though.  And characters that end
		the command line are inserted literally (<Esc>, <CR>, <NL>,
		<C-C>).  A <BS> or CTRL-W could still end the command line
		though, and remaining characters will then be interpreted in
		another mode, which might not be what you intended.
		Special registers:
			'"'	the unnamed register, containing the text of
				the last delete or yank
			'%'	the current file name
			'#'	the alternate file name
			'*'	the clipboard contents (X11: primary selection)
			'+'	the clipboard contents
			'/'	the last search pattern
			':'	the last command-line
			'-'	the last small (less than a line) delete
			'.'	the last inserted text
							*c_CTRL-R_=*
			'='	the expression register: you are prompted to
				enter an expression (see |expression|)
				(doesn't work at the expression prompt; some
				things such as changing the buffer or current
				window are not allowed to avoid side effects)
				When the result is a |List| the items are used
				as lines.  They can have line breaks inside
				too.
				When the result is a Float it's automatically
				converted to a String.
		See |registers| about registers.  {not in Vi}
		Implementation detail: When using the |expression| register
		and invoking setcmdpos(), this sets the position before
		inserting the resulting string.  Use CTRL-R CTRL-R to set the
		position afterwards.

CTRL-R CTRL-F				*c_CTRL-R_CTRL-F* *c_<C-R>_<C-F>*
CTRL-R CTRL-P				*c_CTRL-R_CTRL-P* *c_<C-R>_<C-P>*
CTRL-R CTRL-W				*c_CTRL-R_CTRL-W* *c_<C-R>_<C-W>*
CTRL-R CTRL-A				*c_CTRL-R_CTRL-A* *c_<C-R>_<C-A>*
		Insert the object under the cursor:
			CTRL-F	the Filename under the cursor
			CTRL-P	the Filename under the cursor, expanded with
				'path' as in |gf|
			CTRL-W	the Word under the cursor
			CTRL-A	the WORD under the cursor; see |WORD|

		When 'incsearch' is set the cursor position at the end of the
		currently displayed match is used.  With CTRL-W the part of
		the word that was already typed is not inserted again.

		{not in Vi}
		CTRL-F and CTRL-P: {only when |+file_in_path| feature is
		included}

					*c_CTRL-R_CTRL-R* *c_<C-R>_<C-R>*
					*c_CTRL-R_CTRL-O* *c_<C-R>_<C-O>*
CTRL-R CTRL-R {0-9a-z"%#:-=. CTRL-F CTRL-P CTRL-W CTRL-A}
CTRL-R CTRL-O {0-9a-z"%#:-=. CTRL-F CTRL-P CTRL-W CTRL-A}
		Insert register or object under the cursor.  Works like
		|c_CTRL-R| but inserts the text literally.  For example, if
		register a contains "xy^Hz" (where ^H is a backspace),
		"CTRL-R a" will insert "xz" while "CTRL-R CTRL-R a" will
		insert "xy^Hz".

CTRL-\ e {expr}						*c_CTRL-\_e*
		Evaluate {expr} and replace the whole command line with the
		result.  You will be prompted for the expression, type <Enter>
		to finish it.  It's most useful in mappings though.  See
		|expression|.
		See |c_CTRL-R_=| for inserting the result of an expression.
		Useful functions are |getcmdtype()|, |getcmdline()| and
		|getcmdpos()|.
		The cursor position is unchanged, except when the cursor was
		at the end of the line, then it stays at the end.
		|setcmdpos()| can be used to set the cursor position.
		The |sandbox| is used for evaluating the expression to avoid
		nasty side effects.
		Example: >
			:cmap <F7> <C-\>eAppendSome()<CR>
			:func AppendSome()
			   :let cmd = getcmdline() . " Some()"
			   :" place the cursor on the )
			   :call setcmdpos(strlen(cmd))
			   :return cmd
			:endfunc
<		This doesn't work recursively, thus not when already editing
		an expression.  But it is possible to use in a mapping.

							*c_CTRL-Y*
CTRL-Y		When there is a modeless selection, copy the selection into
		the clipboard. |modeless-selection|
		If there is no selection CTRL-Y is inserted as a character.

CTRL-J					*c_CTRL-J* *c_<NL>* *c_<CR>* *c_CR*
<CR> or <NL>	start entered command
							*c_<Esc>* *c_Esc*
<Esc>		When typed and 'x' not present in 'cpoptions', quit
		Command-line mode without executing.  In macros or when 'x'
		present in 'cpoptions', start entered command.
		Note: If your <Esc> key is hard to hit on your keyboard, train
		yourself to use CTRL-[.
							*c_CTRL-C*
CTRL-C		quit command-line without executing

							*c_<Up>* *c_Up*
<Up>		recall older command-line from history, whose beginning
		matches the current command-line (see below).
		{not available when compiled without the |+cmdline_hist|
		feature}
							*c_<Down>* *c_Down*
<Down>		recall more recent command-line from history, whose beginning
		matches the current command-line (see below).
		{not available when compiled without the |+cmdline_hist|
		feature}

							*c_<S-Up>* *c_<PageUp>*
<S-Up> or <PageUp>
		recall older command-line from history
		{not available when compiled without the |+cmdline_hist|
		feature}
						*c_<S-Down>* *c_<PageDown>*
<S-Down> or <PageDown>
		recall more recent command-line from history
		{not available when compiled without the |+cmdline_hist|
		feature}

CTRL-D		command-line completion (see |cmdline-completion|)
'wildchar' option
		command-line completion (see |cmdline-completion|)
CTRL-N		command-line completion (see |cmdline-completion|)
CTRL-P		command-line completion (see |cmdline-completion|)
CTRL-A		command-line completion (see |cmdline-completion|)
CTRL-L		command-line completion (see |cmdline-completion|)

							*c_CTRL-_*
CTRL-_		a - switch between Hebrew and English keyboard mode, which is
		private to the command-line and not related to hkmap.
		This is useful when Hebrew text entry is required in the
		command-line, searches, abbreviations, etc.  Applies only if
		Vim is compiled with the |+rightleft| feature and the
		'allowrevins' option is set.
		See |rileft.txt|.

		b - switch between Farsi and English keyboard mode, which is
		private to the command-line and not related to fkmap.  In
		Farsi keyboard mode the characters are inserted in reverse
		insert manner.  This is useful when Farsi text entry is
		required in the command-line, searches, abbreviations, etc.
		Applies only if Vim is compiled with the |+farsi| feature.
		See |farsi.txt|.

							*c_CTRL-^*
CTRL-^		Toggle the use of language |:lmap| mappings and/or Input
		Method.
		When typing a pattern for a search command and 'imsearch' is
		not -1, VAL is the value of 'imsearch', otherwise VAL is the
		value of 'iminsert'.
		When language mappings are defined:
		- If VAL is 1 (langmap mappings used) it becomes 0 (no langmap
		  mappings used).
		- If VAL was not 1 it becomes 1, thus langmap mappings are
		  enabled.
		When no language mappings are defined:
		- If VAL is 2 (Input Method is used) it becomes 0 (no input
		  method used)
		- If VAL has another value it becomes 2, thus the Input Method
		  is enabled.
		These language mappings are normally used to type characters
		that are different from what the keyboard produces.  The
		'keymap' option can be used to install a whole number of them.
		When entering a command line, langmap mappings are switched
		off, since you are expected to type a command.  After
		switching it on with CTRL-^, the new state is not used again
		for the next command or Search pattern.
		{not in Vi}

						*c_CTRL-]*
CTRL-]		Trigger abbreviation, without inserting a character.  {not in
		Vi}

For Emacs-style editing on the command-line see |emacs-keys|.

The <Up> and <Down> keys take the current command-line as a search string.
The beginning of the next/previous command-lines are compared with this
string.  The first line that matches is the new command-line.  When typing
these two keys repeatedly, the same string is used again.  For example, this
can be used to find the previous substitute command: Type ":s" and then <Up>.
The same could be done by typing <S-Up> a number of times until the desired
command-line is shown.  (Note: the shifted arrow keys do not work on all
terminals)

							*:his* *:history*
:his[tory]	Print the history of last entered commands.
		{not in Vi}
		{not available when compiled without the |+cmdline_hist|
		feature}

:his[tory] [{name}] [{first}][, [{last}]]
		List the contents of history {name} which can be:
		c[md]	 or :		command-line history
		s[earch] or / or ?	search string history
		e[xpr]	 or =		expression register history
		i[nput]	 or @		input line history
		d[ebug]	 or >		debug command history
		a[ll]			all of the above
		{not in Vi}

		If the numbers {first} and/or {last} are given, the respective
		range of entries from a history is listed.  These numbers can
		be specified in the following form:
							*:history-indexing*
		A positive number represents the absolute index of an entry
		as it is given in the first column of a :history listing.
		This number remains fixed even if other entries are deleted.

		A negative number means the relative position of an entry,
		counted from the newest entry (which has index -1) backwards.

		Examples:
		List entries 6 to 12 from the search history: >
			:history / 6,12
<
		List the recent five entries from all histories: >
			:history all -5,

:keepp[atterns] {command}			*:keepp* *:keeppatterns*
		Execute {command}, without adding anything to the search
		history

==============================================================================
2. Command-line completion				*cmdline-completion*

When editing the command-line, a few commands can be used to complete the
word before the cursor.  This is available for:

- Command names: At the start of the command-line.
- Tags: Only after the ":tag" command.
- File names: Only after a command that accepts a file name or a setting for
  an option that can be set to a file name.  This is called file name
  completion.
- Shell command names: After ":!cmd", ":r !cmd" and ":w !cmd".  $PATH is used.
- Options: Only after the ":set" command.
- Mappings: Only after a ":map" or similar command.
- Variable and function names: Only after a ":if", ":call" or similar command.

When Vim was compiled without the |+cmdline_compl| feature only file names,
directories and help items can be completed.  The number of help item matches
is limited (currently to 300) to avoid a long delay when there are very many
matches.

These are the commands that can be used:

							*c_CTRL-D*
CTRL-D		List names that match the pattern in front of the cursor.
		When showing file names, directories are highlighted (see
		'highlight' option).  Names where 'suffixes' matches are moved
		to the end.
		The 'wildoptions' option can be set to "tagfile" to list the
		file of matching tags.
					*c_CTRL-I* *c_wildchar* *c_<Tab>*
'wildchar' option
		A match is done on the pattern in front of the cursor.  The
		match (if there are several, the first match) is inserted
		in place of the pattern.  (Note: does not work inside a
		macro, because <Tab> or <Esc> are mostly used as 'wildchar',
		and these have a special meaning in some macros.) When typed
		again and there were multiple matches, the next
		match is inserted.  After the last match, the first is used
		again (wrap around).
		The behavior can be changed with the 'wildmode' option.
							*c_CTRL-N*
CTRL-N		After using 'wildchar' which got multiple matches, go to next
		match.  Otherwise recall more recent command-line from history.
<S-Tab>							*c_CTRL-P* *c_<S-Tab>*
CTRL-P		After using 'wildchar' which got multiple matches, go to
		previous match.  Otherwise recall older command-line from
		history.  <S-Tab> only works with the GUI, on the Amiga and
		with MS-DOS.
							*c_CTRL-A*
CTRL-A		All names that match the pattern in front of the cursor are
		inserted.
							*c_CTRL-L*
CTRL-L		A match is done on the pattern in front of the cursor.  If
		there is one match, it is inserted in place of the pattern.
		If there are multiple matches the longest common part is
		inserted in place of the pattern.  If the result is shorter
		than the pattern, no completion is done.
		When 'incsearch' is set, entering a search pattern for "/" or
		"?" and the current match is displayed then CTRL-L will add
		one character from the end of the current match.  If
		'ignorecase' and 'smartcase' are set and the command line has
		no uppercase characters, the added character is converted to
		lowercase.

The 'wildchar' option defaults to <Tab> (CTRL-E when in Vi compatible mode; in
a previous version <Esc> was used).  In the pattern standard wildcards '*' and
'?' are accepted when matching file names.  '*' matches any string, '?'
matches exactly one character.

The 'wildignorecase' option can be set to ignore case in filenames.

If you like tcsh's autolist completion, you can use this mapping:
	:cnoremap X <C-L><C-D>
(Where X is the command key to use, <C-L> is CTRL-L and <C-D> is CTRL-D)
This will find the longest match and then list all matching files.

If you like tcsh's autolist completion, you can use the 'wildmode' option to
emulate it.  For example, this mimics autolist=ambiguous:
	:set wildmode=longest,list
This will find the longest match with the first 'wildchar', then list all
matching files with the next.

							*suffixes*
For file name completion you can use the 'suffixes' option to set a priority
between files with almost the same name.  If there are multiple matches,
those files with an extension that is in the 'suffixes' option are ignored.
The default is ".bak,~,.o,.h,.info,.swp,.obj", which means that files ending
in ".bak", "~", ".o", ".h", ".info", ".swp" and ".obj" are sometimes ignored.

An empty entry, two consecutive commas, match a file name that does not
contain a ".", thus has no suffix.  This is useful to ignore "prog" and prefer
"prog.c".

Examples:

  pattern:	files:				match:	~
   test*	test.c test.h test.o		test.c
   test*	test.h test.o			test.h and test.o
   test*	test.i test.h test.c		test.i and test.c

It is impossible to ignore suffixes with two dots.

If there is more than one matching file (after ignoring the ones matching
the 'suffixes' option) the first file name is inserted.  You can see that
there is only one match when you type 'wildchar' twice and the completed
match stays the same.  You can get to the other matches by entering
'wildchar', CTRL-N or CTRL-P.  All files are included, also the ones with
extensions matching the 'suffixes' option.

To completely ignore files with some extension use 'wildignore'.

To match only files that end at the end of the typed text append a "$".  For
example, to match only files that end in ".c": >
	:e *.c$
This will not match a file ending in ".cpp".  Without the "$" it does match.

The old value of an option can be obtained by hitting 'wildchar' just after
the '='.  For example, typing 'wildchar' after ":set dir=" will insert the
current value of 'dir'.  This overrules file name completion for the options
that take a file name.

If you would like using <S-Tab> for CTRL-P in an xterm, put this command in
your .cshrc: >
	xmodmap -e "keysym Tab = Tab Find"
And this in your .vimrc: >
	:cmap <Esc>[1~ <C-P>

==============================================================================
3. Ex command-lines					*cmdline-lines*

The Ex commands have a few specialties:

							*:quote* *:comment*
'"' at the start of a line causes the whole line to be ignored.  '"'
after a command causes the rest of the line to be ignored.  This can be used
to add comments.  Example: >
	:set ai		"set 'autoindent' option
It is not possible to add a comment to a shell command ":!cmd" or to the
":map" command and a few others, because they see the '"' as part of their
argument.  This is mentioned where the command is explained.

							*:bar* *:\bar*
'|' can be used to separate commands, so you can give multiple commands in one
line.  If you want to use '|' in an argument, precede it with '\'.

These commands see the '|' as their argument, and can therefore not be
followed by another Vim command:
    :argdo
    :autocmd
    :bufdo
    :cdo
    :cfdo
    :command
    :cscope
    :debug
    :folddoopen
    :folddoclosed
    :function
    :global
    :help
    :helpfind
    :lcscope
    :ldo
    :lfdo
    :make
    :normal
    :perl
    :perldo
    :promptfind
    :promptrepl
    :pyfile
    :python
    :registers
    :read !
    :scscope
    :sign
    :tcl
    :tcldo
    :tclfile
    :vglobal
    :windo
    :write !
    :[range]!
    a user defined command without the "-bar" argument |:command|

Note that this is confusing (inherited from Vi): With ":g" the '|' is included
in the command, with ":s" it is not.

To be able to use another command anyway, use the ":execute" command.
Example (append the output of "ls" and jump to the first line): >
	:execute 'r !ls' | '[

There is one exception: When the 'b' flag is present in 'cpoptions', with the
":map" and ":abbr" commands and friends CTRL-V needs to be used instead of
'\'.  You can also use "<Bar>" instead.  See also |map_bar|.

Examples: >
	:!ls | wc		view the output of two commands
	:r !ls | wc		insert the same output in the text
	:%g/foo/p|>		moves all matching lines one shiftwidth
	:%s/foo/bar/|>		moves one line one shiftwidth
	:map q 10^V|		map "q" to "10|"
	:map q 10\| map \ l	map "q" to "10\" and map "\" to "l"
					(when 'b' is present in 'cpoptions')

You can also use <NL> to separate commands in the same way as with '|'.  To
insert a <NL> use CTRL-V CTRL-J.  "^@" will be shown.  Using '|' is the
preferred method.  But for external commands a <NL> must be used, because a
'|' is included in the external command.  To avoid the special meaning of <NL>
it must be preceded with a backslash.  Example: >
	:r !date<NL>-join
This reads the current date into the file and joins it with the previous line.

Note that when the command before the '|' generates an error, the following
commands will not be executed.


Because of Vi compatibility the following strange commands are supported: >
	:|			print current line (like ":p")
	:3|			print line 3 (like ":3p")
	:3			goto line 3

A colon is allowed between the range and the command name.  It is ignored
(this is Vi compatible).  For example: >
	:1,$:s/pat/string

When the character '%' or '#' is used where a file name is expected, they are
expanded to the current and alternate file name (see the chapter "editing
files" |:_%| |:_#|).

Embedded spaces in file names are allowed on the Amiga if one file name is
expected as argument.  Trailing spaces will be ignored, unless escaped with a
backslash or CTRL-V.  Note that the ":next" command uses spaces to separate
file names.  Escape the spaces to include them in a file name.  Example: >
	:next foo\ bar goes\ to school\
starts editing the three files "foo bar", "goes to" and "school ".

When you want to use the special characters '"' or '|' in a command, or want
to use '%' or '#' in a file name, precede them with a backslash.  The
backslash is not required in a range and in the ":substitute" command.
See also |`=|.

							*:_!*
The '!' (bang) character after an Ex command makes the command behave in a
different way.  The '!' should be placed immediately after the command, without
any blanks in between.  If you insert blanks the '!' will be seen as an
argument for the command, which has a different meaning.  For example:
	:w! name	write the current buffer to file "name", overwriting
			any existing file
	:w !name	send the current buffer as standard input to command
			"name"

==============================================================================
4. Ex command-line ranges	*cmdline-ranges* *[range]* *E16*

Some Ex commands accept a line range in front of them.  This is noted as
[range].  It consists of one or more line specifiers, separated with ',' or
';'.

The basics are explained in section |10.3| of the user manual.

						*:,* *:;*
When separated with ';' the cursor position will be set to that line
before interpreting the next line specifier.  This doesn't happen for ','.
Examples: >
   4,/this line/
<	from line 4 till match with "this line" after the cursor line. >
   5;/that line/
<	from line 5 till match with "that line" after line 5.

The default line specifier for most commands is the cursor position, but the
commands ":write" and ":global" have the whole file (1,$) as default.

If more line specifiers are given than required for the command, the first
one(s) will be ignored.

Line numbers may be specified with:		*:range* *E14* *{address}*
	{number}	an absolute line number
	.		the current line			  *:.*
	$		the last line in the file		  *:$*
	%		equal to 1,$ (the entire file)		  *:%*
	't		position of mark t (lowercase)		  *:'*
	'T		position of mark T (uppercase); when the mark is in
			another file it cannot be used in a range
	/{pattern}[/]	the next line where {pattern} matches	  *:/*
	?{pattern}[?]	the previous line where {pattern} matches *:?*
	\/		the next line where the previously used search
			pattern matches
	\?		the previous line where the previously used search
			pattern matches
	\&		the next line where the previously used substitute
			pattern matches

Each may be followed (several times) by '+' or '-' and an optional number.
This number is added or subtracted from the preceding line number.  If the
number is omitted, 1 is used.

The "/" and "?" after {pattern} are required to separate the pattern from
anything that follows.

The "/" and "?" may be preceded with another address.  The search starts from
there.  The difference from using ';' is that the cursor isn't moved.
Examples: >
	/pat1//pat2/	Find line containing "pat2" after line containing
			"pat1", without moving the cursor.
	7;/pat2/	Find line containing "pat2", after line 7, leaving
			the cursor in line 7.

The {number} must be between 0 and the number of lines in the file.  When
using a 0 (zero) this is interpreted as a 1 by most commands.  Commands that
use it as a count do use it as a zero (|:tag|, |:pop|, etc).  Some commands
interpret the zero as "before the first line" (|:read|, search pattern, etc).

Examples: >
	.+3		three lines below the cursor
	/that/+1	the line below the next line containing "that"
	.,$		from current line until end of file
	0;/that		the first line containing "that", also matches in the
			first line.
	1;/that		the first line after line 1 containing "that"

Some commands allow for a count after the command.  This count is used as the
number of lines to be used, starting with the line given in the last line
specifier (the default is the cursor line).  The commands that accept a count
are the ones that use a range but do not have a file name argument (because
a file name can also be a number).

Examples: >
	:s/x/X/g 5	substitute 'x' by 'X' in the current line and four
			following lines
	:23d 4		delete lines 23, 24, 25 and 26


Folds and Range

When folds are active the line numbers are rounded off to include the whole
closed fold.  See |fold-behavior|.


Reverse Range						*E493*

A range should have the lower line number first.  If this is not the case, Vim
will ask you if it should swap the line numbers.
	Backwards range given, OK to swap ~
This is not done within the global command ":g".

You can use ":silent" before a command to avoid the question, the range will
always be swapped then.


Count and Range						*N:*

When giving a count before entering ":", this is translated into:
		:.,.+(count - 1)
In words: The 'count' lines at and after the cursor.  Example: To delete
three lines: >
		3:d<CR>		is translated into: .,.+2d<CR>
<

Visual Mode and Range					*v_:*

{Visual}:	Starts a command-line with the Visual selected lines as a
		range.  The code `:'<,'>` is used for this range, which makes
		it possible to select a similar line from the command-line
		history for repeating a command on different Visually selected
		lines.
		When Visual mode was already ended, a short way to use the
		Visual area for a range is `:*`.  This requires that "*" does
		not appear in 'cpo', see |cpo-star|.  Otherwise you will have
		to type `:'<,'>`


==============================================================================
5. Ex command-line flags				*ex-flags*

These flags are supported by a selection of Ex commands.  They print the line
that the cursor ends up after executing the command:

	l	output like for |:list|
	#	add line number
	p	output like for |:print|

The flags can be combined, thus "l#" uses both a line number and |:list| style
output.

==============================================================================
6. Ex special characters				*cmdline-special*

Note: These are special characters in the executed command line.  If you want
to insert special things while typing you can use the CTRL-R command.  For
example, "%" stands for the current file name, while CTRL-R % inserts the
current file name right away.  See |c_CTRL-R|.

Note:  If you want to avoid the effects of special characters in a Vim script
you may want to use |fnameescape()|.  Also see |`=|.


In Ex commands, at places where a file name can be used, the following
characters have a special meaning.  These can also be used in the expression
function |expand()|.
	%	Is replaced with the current file name.		  *:_%* *c_%*
	#	Is replaced with the alternate file name.	  *:_#* *c_#*
		This is remembered for every window.
	#n	(where n is a number) is replaced with		  *:_#0* *:_#n*
		the file name of buffer n.  "#0" is the same as "#".     *c_#n*
	##	Is replaced with all names in the argument list	  *:_##* *c_##*
		concatenated, separated by spaces.  Each space in a name
		is preceded with a backslash.
	#<n	(where n is a number > 0) is replaced with old	  *:_#<* *c_#<*
		file name n.  See |:oldfiles| or |v:oldfiles| to get the
		number.							*E809*
		{only when compiled with the |+eval| and |+viminfo| features}

Note that these, except "#<n", give the file name as it was typed.  If an
absolute path is needed (when using the file name from a different directory),
you need to add ":p".  See |filename-modifiers|.

The "#<n" item returns an absolute path, but it will start with "~/" for files
below your home directory.

Note that backslashes are inserted before spaces, so that the command will
correctly interpret the file name.  But this doesn't happen for shell
commands.  For those you probably have to use quotes (this fails for files
that contain a quote and wildcards): >
	:!ls "%"
	:r !spell "%"

To avoid the special meaning of '%' and '#' insert a backslash before it.
Detail: The special meaning is always escaped when there is a backslash before
it, no matter how many backslashes.
	you type:		result	~
	   #			alternate.file
	   \#			#
	   \\#			\#
Also see |`=|.

			       *:<cword>* *:<cWORD>* *:<cfile>* *<cfile>*
			       *:<sfile>* *<sfile>* *:<afile>* *<afile>*
			       *:<abuf>* *<abuf>* *:<amatch>* *<amatch>*
			       *<slnum>* *E495* *E496* *E497* *E499* *E500*
Note: these are typed literally, they are not special keys!
	<cword>    is replaced with the word under the cursor (like |star|)
	<cWORD>    is replaced with the WORD under the cursor (see |WORD|)
	<cfile>    is replaced with the path name under the cursor (like what
		   |gf| uses)
	<afile>    When executing autocommands, is replaced with the file name
		   for a file read or write.
	<abuf>     When executing autocommands, is replaced with the currently
		   effective buffer number (for ":r file" and ":so file" it is
		   the current buffer, the file being read/sourced is not in a
		   buffer).
	<amatch>   When executing autocommands, is replaced with the match for
		   which this autocommand was executed.  It differs from
		   <afile> only when the file name isn't used to match with
		   (for FileType, Syntax and SpellFileMissing events).
	<sfile>    When executing a ":source" command, is replaced with the
		   file name of the sourced file.  *E498*
		   When executing a function, is replaced with
		   "function {function-name}"; function call nesting is
		   indicated like this:
		   "function {function-name1}..{function-name2}".  Note that
		   filename-modifiers are useless when <sfile> is used inside
		   a function.
	<slnum>	   When executing a ":source" command, is replaced with the
	           line number.  *E842*
		   When executing a function it's the line number relative to
		   the start of the function.

							 *filename-modifiers*
*:_%:* *::8* *::p* *::.* *::~* *::h* *::t* *::r* *::e* *::s* *::gs* *::S*
     *%:8* *%:p* *%:.* *%:~* *%:h* *%:t* *%:r* *%:e* *%:s* *%:gs* *%:S*
The file name modifiers can be used after "%", "#", "#n", "<cfile>", "<sfile>",
"<afile>" or "<abuf>".  They are also used with the |fnamemodify()| function.
These are not available when Vim has been compiled without the |+modify_fname|
feature.
These modifiers can be given, in this order:
	:p	Make file name a full path.  Must be the first modifier.  Also
		changes "~/" (and "~user/" for Unix and VMS) to the path for
		the home directory.  If the name is a directory a path
		separator is added at the end.  For a file name that does not
		exist and does not have an absolute path the result is
		unpredictable.  On MS-Windows an 8.3 filename is expanded to
		the long name.
	:8	Converts the path to 8.3 short format (currently only on
		MS-Windows).  Will act on as much of a path that is an
		existing path.
	:~	Reduce file name to be relative to the home directory, if
		possible.  File name is unmodified if it is not below the home
		directory.
	:.	Reduce file name to be relative to current directory, if
		possible.  File name is unmodified if it is not below the
		current directory.
		For maximum shortness, use ":~:.".
	:h	Head of the file name (the last component and any separators
		removed).  Cannot be used with :e, :r or :t.
		Can be repeated to remove several components at the end.
		When the file name ends in a path separator, only the path
		separator is removed.  Thus ":p:h" on a directory name results
		on the directory name itself (without trailing slash).
		When the file name is an absolute path (starts with "/" for
		Unix; "x:\" for MS-DOS, WIN32, OS/2; "drive:" for Amiga), that
		part is not removed.  When there is no head (path is relative
		to current directory) the result is empty.
	:t	Tail of the file name (last component of the name).  Must
		precede any :r or :e.
	:r	Root of the file name (the last extension removed).  When
		there is only an extension (file name that starts with '.',
		e.g., ".vimrc"), it is not removed.  Can be repeated to remove
		several extensions (last one first).
	:e	Extension of the file name.  Only makes sense when used alone.
		When there is no extension the result is empty.
		When there is only an extension (file name that starts with
		'.'), the result is empty.  Can be repeated to include more
		extensions.  If there are not enough extensions (but at least
		one) as much as possible are included.
	:s?pat?sub?
		Substitute the first occurrence of "pat" with "sub".  This
		works like the |:s| command.  "pat" is a regular expression.
		Any character can be used for '?', but it must not occur in
		"pat" or "sub".
		After this, the previous modifiers can be used again.  For
		example ":p", to make a full path after the substitution.
	:gs?pat?sub?
		Substitute all occurrences of "pat" with "sub".  Otherwise
		this works like ":s".
	:S	Escape special characters for use with a shell command (see
		|shellescape()|). Must be the last one. Examples: >
		    :!dir <cfile>:S
		    :call system('chmod +w -- ' . expand('%:S'))

Examples, when the file name is "src/version.c", current dir
"/home/mool/vim": >
  :p			/home/mool/vim/src/version.c
  :p:.				       src/version.c
  :p:~				 ~/vim/src/version.c
  :h				       src
  :p:h			/home/mool/vim/src
  :p:h:h		/home/mool/vim
  :t					   version.c
  :p:t					   version.c
  :r				       src/version
  :p:r			/home/mool/vim/src/version
  :t:r					   version
  :e						   c
  :s?version?main?		       src/main.c
  :s?version?main?:p	/home/mool/vim/src/main.c
  :p:gs?/?\\?		\home\mool\vim\src\version.c

Examples, when the file name is "src/version.c.gz": >
  :p			/home/mool/vim/src/version.c.gz
  :e						     gz
  :e:e						   c.gz
  :e:e:e					   c.gz
  :e:e:r					   c
  :r				       src/version.c
  :r:e						   c
  :r:r				       src/version
  :r:r:r			       src/version
<
					*extension-removal* *:_%<*
If a "<" is appended to "%", "#", "#n" or "CTRL-V p" the extension of the file
name is removed (everything after and including the last '.' in the file
name).  This is included for backwards compatibility with version 3.0, the
":r" form is preferred.  Examples: >

	%		current file name
	%<		current file name without extension
	#		alternate file name for current window
	#<		idem, without extension
	#31		alternate file number 31
	#31<		idem, without extension
	<cword>		word under the cursor
	<cWORD>		WORD under the cursor (see |WORD|)
	<cfile>		path name under the cursor
	<cfile><	idem, without extension

Note: Where a file name is expected wildcards expansion is done.  On Unix the
shell is used for this, unless it can be done internally (for speed).
Unless in |restricted-mode|, backticks work also, like in >
	:n `echo *.c`
But expansion is only done if there are any wildcards before expanding the
'%', '#', etc..  This avoids expanding wildcards inside a file name.  If you
want to expand the result of <cfile>, add a wildcard character to it.
Examples: (alternate file name is "?readme?")
	command		expands to  ~
	:e #		:e ?readme?
	:e `ls #`	:e {files matching "?readme?"}
	:e #.*		:e {files matching "?readme?.*"}
	:cd <cfile>	:cd {file name under cursor}
	:cd <cfile>*	:cd {file name under cursor plus "*" and then expanded}
Also see |`=|.

When the expanded argument contains a "!" and it is used for a shell command
(":!cmd", ":r !cmd" or ":w !cmd"), the "!" is escaped with a backslash to
avoid it being expanded into a previously used command.  When the 'shell'
option contains "sh", this is done twice, to avoid the shell trying to expand
the "!".

							*filename-backslash*
For filesystems that use a backslash as directory separator (MS-DOS, Windows,
OS/2), it's a bit difficult to recognize a backslash that is used to escape
the special meaning of the next character.  The general rule is: If the
backslash is followed by a normal file name character, it does not have a
special meaning.  Therefore "\file\foo" is a valid file name, you don't have
to type the backslash twice.

An exception is the '$' sign.  It is a valid character in a file name.  But
to avoid a file name like "$home" to be interpreted as an environment variable,
it needs to be preceded by a backslash.  Therefore you need to use "/\$home"
for the file "$home" in the root directory.  A few examples:

	FILE NAME	INTERPRETED AS	~
	$home		expanded to value of environment var $home
	\$home		file "$home" in current directory
	/\$home		file "$home" in root directory
	\\$home		file "\\", followed by expanded $home

Also see |`=|.

==============================================================================
7. Command-line window				*cmdline-window* *cmdwin*
							*command-line-window*
In the command-line window the command line can be edited just like editing
text in any window.  It is a special kind of window, because you cannot leave
it in a normal way.
{not available when compiled without the |+cmdline_hist| or |+vertsplit|
feature}


OPEN						*c_CTRL-F* *q:* *q/* *q?*

There are two ways to open the command-line window:
1. From Command-line mode, use the key specified with the 'cedit' option.
   The default is CTRL-F when 'compatible' is not set.
2. From Normal mode, use the "q:", "q/" or "q?" command.
   This starts editing an Ex command-line ("q:") or search string ("q/" or
   "q?").  Note that this is not possible while recording is in progress (the
   "q" stops recording then).

When the window opens it is filled with the command-line history.  The last
line contains the command as typed so far.  The left column will show a
character that indicates the type of command-line being edited, see
|cmdwin-char|.

Vim will be in Normal mode when the editor is opened, except when 'insertmode'
is set.

The height of the window is specified with 'cmdwinheight' (or smaller if there
is no room).  The window is always full width and is positioned just above the
command-line.


EDIT

You can now use commands to move around and edit the text in the window.  Both
in Normal mode and Insert mode.

It is possible to use ":", "/" and other commands that use the command-line,
but it's not possible to open another command-line window then.  There is no
nesting.
							*E11*
The command-line window is not a normal window.  It is not possible to move to
another window or edit another buffer.  All commands that would do this are
disabled in the command-line window.  Of course it _is_ possible to execute
any command that you entered in the command-line window.  Other text edits are
discarded when closing the window.


CLOSE							*E199*

There are several ways to leave the command-line window:

<CR>		Execute the command-line under the cursor.  Works both in
		Insert and in Normal mode.
CTRL-C		Continue in Command-line mode.  The command-line under the
		cursor is used as the command-line.  Works both in Insert and
		in Normal mode.  ":close" also works.  There is no redraw,
		thus the window will remain visible.
:quit		Discard the command line and go back to Normal mode.
		":exit", ":xit" and CTRL-\ CTRL-N also work.
:qall		Quit Vim, unless there are changes in some buffer.
:qall!		Quit Vim, discarding changes to any buffer.

Once the command-line window is closed the old window sizes are restored.  The
executed command applies to the window and buffer where the command-line was
started from.  This works as if the command-line window was not there, except
that there will be an extra screen redraw.
The buffer used for the command-line window is deleted.  Any changes to lines
other than the one that is executed with <CR> are lost.

If you would like to execute the command under the cursor and then have the
command-line window open again, you may find this mapping useful: >

	:autocmd CmdwinEnter * map <buffer> <F5> <CR>q:


VARIOUS

The command-line window cannot be used:
- when there already is a command-line window (no nesting)
- for entering an encryption key or when using inputsecret()
- when Vim was not compiled with the |+vertsplit| feature

Some options are set when the command-line window is opened:
'filetype'	"vim", when editing an Ex command-line; this starts Vim syntax
		highlighting if it was enabled
'rightleft'	off
'modifiable'	on
'buftype'	"nofile"
'swapfile'	off

It is allowed to write the buffer contents to a file.  This is an easy way to
save the command-line history and read it back later.

If the 'wildchar' option is set to <Tab>, and the command-line window is used
for an Ex command, then two mappings will be added to use <Tab> for completion
in the command-line window, like this: >
	:imap <buffer> <Tab> <C-X><C-V>
	:nmap <buffer> <Tab> a<C-X><C-V>
Note that hitting <Tab> in Normal mode will do completion on the next
character.  That way it works at the end of the line.
If you don't want these mappings, disable them with: >
	au CmdwinEnter [:>] iunmap <Tab>
	au CmdwinEnter [:>] nunmap <Tab>
You could put these lines in your vimrc file.

While in the command-line window you cannot use the mouse to put the cursor in
another window, or drag statuslines of other windows.  You can drag the
statusline of the command-line window itself and the statusline above it.
Thus you can resize the command-line window, but not others.

The |getcmdwintype()| function returns the type of the command-line being
edited as described in |cmdwin-char|.


AUTOCOMMANDS

Two autocommand events are used: |CmdwinEnter| and |CmdwinLeave|.  Since this
window is of a special type, the WinEnter, WinLeave, BufEnter and BufLeave
events are not triggered.  You can use the Cmdwin events to do settings
specifically for the command-line window.  Be careful not to cause side
effects!
Example: >
	:au CmdwinEnter :  let b:cpt_save = &cpt | set cpt=.
	:au CmdwinLeave :  let &cpt = b:cpt_save
This sets 'complete' to use completion in the current window for |i_CTRL-N|.
Another example: >
	:au CmdwinEnter [/?]  startinsert
This will make Vim start in Insert mode in the command-line window.

						*cmdwin-char*
The character used for the pattern indicates the type of command-line:
	:	normal Ex command
	>	debug mode command |debug-mode|
	/	forward search string
	?	backward search string
	=	expression for "= |expr-register|
	@	string for |input()|
	-	text for |:insert| or |:append|

 vim:tw=78:ts=8:ft=help:norl:
