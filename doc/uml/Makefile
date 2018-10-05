DIAGRAMS := simplewrite.utxt delayedwrite.utxt

all: ${DIAGRAMS}

%.utxt: %.uml
	plantuml -utxt -o "." $<
