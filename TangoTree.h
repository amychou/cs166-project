#ifndef SplayTree_Included
#define SplayTree_Included

#include <string>
#include <vector>

/**
 * A type representing a binary search tree backed by a splay tree.
 */
class TangoTree {
public:
    /**
     * Given a list of the future access probabilities of the elements 0, 1, 2,
     * ..., weights.size() - 1, constructs a new tango tree holding those
     * elements.
     *
     * Because tango trees rearrange their elements in response to queries, we
     * ignore the assigned probabilities here and just build a BST storing the
     * keys 0, 1, 2, ..., weights.size() - 1.
     */
    explicit TangoTree(const std::vector<double>& weights);

    /**
     * Cleans up all memory allocated by the tree. Remember that destructors are
     * invoked automatically in C++, so you should never need to directly invoke
     * this member function.
     */
    ~TangoTree();

    /**
     * Searches the tango tree for the given key, returning whether or not that
     * key is present in the tree.
     */
    bool contains(std::size_t key) const;

    /**
     * Returns a human-readable name of this type.
     */
    static std::string name() {
        return "Tango Tree";
    }

private:
    enum Color {RED, BLACK};
    struct Node {
        std::size_t key;

        Node* left;
        Node* right;
        Node* parent;

        int subtree_size;

        bool marked;
        Color color;

        int depth, min_depth, max_depth;
    };

    /* Pointer to the root of the tree - which will likely change a lot!
     *
     * This variable is marked mutable. The mutable keyword in C++ means that
     * this data member can be modified by const member functions. As a result,
     * even though the contains() member function is const, it's still allowed
     * to change this value.
     *
     * The mutable keyword, more generally, is used in contexts where you have an
     * operation that doesn't logically change the internal state of an object,
     * but which might change the actual bits of that object.
     */
    mutable Node* root;

    /* Constructs an initial tree for all the keys in the given range. This is
     * taken verbatim from the perfectly-balanced tree.
     */
    static Node* treeFor(std::size_t low, std::size_t high);

    // TODO: Add any necessary new types or fields here.
    Node* cutAndJoin(Node* node)const;

    Node* cut(Node* node, int depth) const;
    Node* join(Node* n1, Node* n2, int depth) const;
    Node* split(Node* n1, Node* n2) const;
    Node* merge(Node* n) const;

    Node* findMinWithDepth(Node* n, int depth) const;
    Node* findMaxWithDepth(Node* n, int depth) const;

    Node* getPredecessorByNode(Node* n) const;
    Node* getSuccessorByNode(Node* n) const;

    Node* findMarkedPredecessor(Node* node, size_t key) const;

    void updateMinMaxPath(Node* n) const;

    bool isLeaf(Node* n) const;

    /* Fun with C++: these next two lines disable implicitly-generated copy
     * functions that would otherwise cause weird errors if you tried to
     * implicitly copy an object of this type. You don't need to touch these
     * lines.
     */
    TangoTree(TangoTree const &) = delete;
    void operator=(TangoTree const &) = delete;
};

#endif
