#ifndef AUTOMATA_H
#define AUTOMATA_H

#include <stdbool.h>
#include "util.h"

struct AutomataLink_t;
struct AutoamtaNode_t;

// AutomataMatcher functions accept a string and a length, returning true if
// the input character matches some criteria.
typedef bool (*AutomataMatcher)(struct AutomataLink_t*, char);

typedef enum {
	AUTOMATA_MATCHER_EPSILON,     /* epsilon-match */
	AUTOMATA_MATCHER_CLASS,       /* character class */
	AUTOMATA_MATCHER_PALINDROME   /* palindrome */
} AutomataMatcherType;

// AutomataLink describes a state transition between two automata nodes. It
// will activate the node it links to if a character matching
typedef struct AutomataLink_t {

	// Matcher is the criteria which causes the link to propagate active
	// states.
	AutomataMatcher Matcher;

	// Type is used to determine which of the remaining struct fields are
	// used.
	AutomataMatcherType Type;

	// Text is used by palindrome matchers to record the history of what
	// they have seen, and by class matchers to store the list of accepted
	// characters.
	char* Text;

	// NText is the length of the Text field.
	size_t NText;

	// Target is the node which should be activated if this link matches,
	// and the source node is active.
	struct AutomataNode_t* Target;

	// Next is used to allow AutomataLink to be used in a linked list.
	struct AutomataLink_t* next;

} AutomataLink;

// AutomataNode represents one node in the generated automata.
typedef struct AutomataNode_t {

	// Accepting is true if this node is an accepting state.
	bool Accepting;

	// Active is true if this node is currently active.
	bool Active;

	// Links stores a list of outgoing links from this automata state.
	struct AutomataLink_t * Links;

} AutomataNode;

// AutomataNewEpsilonMatcher allocates a new link object accepting any symbol,
// or crashes with an error.
AutomataLink* AutomataNewEpsilonMatcher();

// AutomataNewClassMatcher allocates a new link object which will match any
// character in the string class. nclass must be the length of the class
// string. The lifecycle of the given string is handled by the instantiated
// link object.
AutomataLink* AutomataNewClassMatcher(char* class, size_t nclass);

// AutomataNewPalindromeMatcher allocates a new link object which will
// match a palindrome.
AutomataLink* AutomataNewPalindromeMatcher();

// AutomataFreeLink will safely free a link object, including the text if it
// has been set. If Target is set, the target will be free-ed using
// AutomataFreeNode(). If next is set, all other links in the list will
// also be free-ed using AutomataFreeLink.
void AutomataFreeLink(struct AutomataLink_t* link);

// AutomataClassMatch implements AutomataMatcher, and will return
// true if the given symbol is contained by link->Text.
bool AutomataClassMatch(struct AutomataLink_t* link, char symbol);

// AutomataPalindromeMatch implements AutomataMatcher. It appends the
// symbol to link->Text, and returns true if link->Text ends with a
// palindrome of length >=1.
bool AutomataPalindromeMatch(struct AutomataLink_t* link, char symbol);

// AutomataEpsilonMatch implements AutomataMatcher. It returns true
// unconditionally.
bool AutomataEpsilonMatch(struct AutomataLink_t* link, char symbol);

// AutomataNewNode instantiates a new node object. If accepting is asserted,
// then the node will be considered an accepting state.
AutomataNode* AutomataNewNode(bool accepting);

// AutomataFreeNode will safely free the specified node. It will first perform
// a depth-first traversal of all linked nodes, freeing them and all reachable
// links.
void AutomataFreeNode(struct AutomataNode_t* node);


#endif
