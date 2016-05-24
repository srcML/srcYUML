grammar srcYUML2graphViz;

yuml
	: (node | relationship)+ EOF
	;

relationship
	: (node (aggregation | composition | realization | generalization)  node) NEWLINE*
	;

node
	: '[' text ']' NEWLINE*
	;

aggregation
	: text '+' text '-' text '>' text
	;

composition
	: text '+' text '+' text '-' text '>'
	;

realization
	: text '^' text '-' text '.' text '-'
 	;
 
 generalization
 	: text '^' text '-' text
 	; 

 text
 	: (LETTER | NUMBER | ('|') | ('-') | ('+') | ('#') | ('<') | ('>') | ';' | '(' | ')' | ':' | ' ' | '*')*
 	;

 LETTER
 	: [a-zA-Z]
 	;

 NUMBER
 	: [0-9]
 	;

NEWLINE
	: '\n'
	;