#include "automata.h"

AutomataLink* AutomataNewEpsilonMatcher() {
	AutomataLink* l = MustMalloc(sizeof(AutomataLink));
	l->Matcher =AutomataEpsilonMatch;
	l->Type = AUTOMATA_MATCHER_EPSILON;
	l->Text = NULL;
	l->NText = 0;
	l->Target = NULL;
	l->next = NULL;
	dbprintf("created new epsilon matcher: %p\n", (void*) l);
	return l;
}

AutomataLink* AutomataNewClassMatcher(char* class, size_t nclass) {
	AutomataLink* l = AutomataNewEpsilonMatcher();
	l->Type = AUTOMATA_MATCHER_CLASS;
	l->Text = class;
	l->NText = nclass;
	dbprintf("created new class matcher for class '%s': %p\n", class, (void*) l);
	return l;
}

AutomataLink* AutomataNewPalindromeMatcher() {
	AutomataLink* l = AutomataNewEpsilonMatcher();
	l->Type = AUTOMATA_MATCHER_PALINDROME;
	l->Text = MustMalloc(sizeof(char));
	l->Text[0] = 0;
	l->NText = 0;
	dbprintf("created new palindrome matcher: %p\n", (void*) l);
	return l;
}

void AutomataFreeLink(struct AutomataLink_t* link) {
	dbprintf("free-ing link %p\n", (void*) link);

	if (link->next != NULL) {
		dbprintf("link %p contains non-NULL next field %p, free-ing it first...\n", (void*) link, (void*) link->next);
		AutomataFreeLink(link);
	}

	if (link->Text != NULL) {
		dbprintf("free-ing text field for link %p\n", (void*) link);
		free(link->Text);
	}

	if (link->Target != NULL) {
		dbprintf("link %p contains non-NULL Target field %p, freeing it...\n", (void*) link, (void*) link->Target);
		AutomataFreeNode(link->Target);
	}

	dbprintf("link fields for %p free-ed.\n", (void*) link);
	free(link);

}


bool AutomataClassMatch(struct AutomataLink_t* link, char symbol) {
	dbprintf("matching symbol '%c' link %p character class '%s'\n", symbol, (void*) link, link->Text);
	for (char* c = link->Text; c < (link->Text + link->NText) ; c++) {
		if (*c == symbol) {
			dbprintf("\tMATCH\n");
			return true;
		}
	}
	dbprintf("\tNO MATCH\n");
	return false;
}

bool AutomataPalindromeMatch(struct AutomataLink_t* link, char symbol) {
	dbprintf("checking for palindrome match for link %p and symbol '%c'\n", (void*) link, symbol);
	// TODO
	return false;
}

bool AutomataEpsilonMatch(struct AutomataLink_t* link, char symbol) {
	UNUSED(link);
	UNUSED(symbol);
	dbprintf("epsilon-match for link %p\n", (void*) link);
	return true;
}


AutomataNode* AutomataNewNode(bool accepting) {
	AutomataNode* n = MustMalloc(sizeof(AutomataNode));
	n->Active = false;
	n->Accepting = accepting;
	n->Links = NULL;
	dbprintf("created new %s node %p\n", (accepting) ? "accepting" : "non-accepting", (void*) n);
	return n;
}

void AutomataFreeNode(struct AutomataNode_t* node) {
	dbprintf("free-ing node %p\n", (void*) node);
	if (node->Links != NULL) {
		dbprintf("node %p has links, free-ing those first\n", (void*) node);
		AutomataFreeLink(node->Links); // handles list traversal automatically
	}
	free(node);
}

bool AutomataNextState(struct AutomataNode_t* node, char symbol) {
	return false;
}

// Internal logic for traversing the automata, cursor should be 0 initially,
// and is used by recursive calls to track the next unused index in the results
// array.
//
// This function assumes that the node in the argument list has already been
// added to the list, and only traverses linked nodes.
static void _automataCollectConnected(struct AutomataNode_t* node, unsigned int* count, struct AutomataNode_t*** results, int* cursor) {

	// count the links
	int nLinks = 0;
	for (AutomataLink* l = node->Links ; l != NULL ; l = l->next) {
		if (l->Target != NULL) { nLinks ++; }
	}

	// if there are no outgoing links, we can return early
	if (nLinks == 0) { return; }

	// expand the results array
	*count += nLinks;
	*results = realloc(*results, *count*sizeof(AutomataNode*));
	if (*results == NULL) {
		err(1, "failed to expand results array");
	}

	// populate results
	for (AutomataLink* l = node->Links ; l != NULL ; l = l->next) {
		if (l->Target != NULL) {
			(*results)[*cursor] = l->Target;
			(*cursor)++;
		}
	}

	// descend
	for (AutomataLink* l = node->Links ; l != NULL ; l = l->next) {
		_automataCollectConnected(l->Target, count, results, cursor);
	}
}

void AutomataCollectConnected(struct AutomataNode_t* node, unsigned int* count, struct AutomataNode_t*** results) {
	*results = MustMalloc(sizeof(AutomataNode**));
	*count = 1;
	*results[0] = node;
	int cursor = 1;
	_automataCollectConnected(node, count, results, &cursor);
}

void AutomataLinkNodes(struct AutomataLink_t* link, struct AutomataNode_t* from, struct AutomataNode_t* to) {

	// insert the link into the origin node
	if (from->Links == NULL) {
		from->Links = link;
	} else {
		for (AutomataLink* l = from->Links ; l != NULL ; l = l->next) {
			if (l->next == NULL) {
				l->next = link;
				break;
			}
		}
	}

	link->Target = to;
}

void AutomataDumpGraphviz(struct AutomataNode_t* node, FILE* fd) {
	AutomataNode* cursor;
	fprintf(fd, "digraph G {\n");
	int num = 0;
	AutomataTraverseConnected(node, cursor,
		fprintf(fd, "    \"%p\" [label=\"%d\" shape=\"%s\" color=\"%s\"]\n",
			(void*) cursor, num,
			(cursor->Accepting) ? "doublecircle" : "circle",
			(cursor->Active) ? "red" : "black");
		num++;
	);

	AutomataTraverseConnected(node, cursor,
		for (AutomataLink* l = cursor->Links ; l != NULL ; l = l->next) {
			if (l->Type == AUTOMATA_MATCHER_EPSILON) {
				fprintf(fd, "    \"%p\" -> \"%p\" [ label = epsilon ]\n",
					(void*) cursor, (void*) l->Target);
			} else if (l->Type == AUTOMATA_MATCHER_CLASS) {
				fprintf(fd, "    \"%p\" -> \"%p\" [ label = \"class '%s'\" ]\n",
					(void*) cursor, (void*) l->Target, l->Text);
			} else if (l->Type == AUTOMATA_MATCHER_PALINDROME) {
				fprintf(fd, "    \"%p\" -> \"%p\" [ label = palindrome ]\n",
					(void*) cursor, (void*) l->Target);
			}
		}
	);

	fprintf(fd, "}\n");
}
