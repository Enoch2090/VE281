#include <tuple>
#include <vector>
#include <algorithm>
#include <cassert>
#include <stdexcept>

/**
 * An abstract template base of the KDTree class
 */
template<typename...>
class KDTree;

/**
 * A partial template specialization of the KDTree class
 * The time complexity of functions are based on n and k
 * n is the size of the KDTree
 * k is the number of dimensions
 * @typedef Key         key type
 * @typedef Value       value type
 * @typedef Data        key-value pair
 * @static  KeySize     k (number of dimensions)
 */
template<typename ValueType, typename... KeyTypes>
class KDTree<std::tuple<KeyTypes...>, ValueType> {
public:
    typedef std::tuple<KeyTypes...> Key;
    typedef ValueType Value;
    typedef std::pair<const Key, Value> Data;
    typedef typename std::vector<std::pair<Key, Value>>::iterator inputVecIt;
    static inline constexpr size_t KeySize = std::tuple_size<Key>::value;
    static_assert(KeySize > 0, "Can not construct KDTree with zero dimension");
protected:
    struct Node {
        Data data;
        Node* parent;
        Node* left = nullptr;
        Node* right = nullptr;

        Node(const Key& key, const Value& value, Node* parent) : data(key, value), parent(parent) {}

        const Key& key() { return data.first; }

        Value& value() { return data.second; }

        bool isLeafNode() { return (left==nullptr) && (right==nullptr); }

        bool isLeftChild() { return (parent!=nullptr) && (this==this->parent->left); }

        bool isRightChild() { return (parent!=nullptr) && (this==this->parent->right); }

        bool hasLeftSubTree() { return !(left==nullptr); }

        bool hasRightSubTree() { return !(right==nullptr); }

        bool isRightMostNode() {
            //if (!this->isLeafNode()) { return false; }
            Node* thisNode = this;
            while (thisNode->parent) {
                if (!(thisNode==thisNode->parent->right)) { return false; }
                thisNode = thisNode->parent;
            }
            return true;
        }

        bool isLeftMostNode() {
            //if (!this->isLeafNode()) { return false; }
            Node* thisNode = this;
            while (thisNode->parent) {
                if (!(thisNode==thisNode->parent->left)) { return false; }
                thisNode = thisNode->parent;
            }
            return true;
        }
    };

public:
    /**
     * A bi-directional iterator for the KDTree
     * ! ONLY NEED TO MODIFY increment and decrement !
     */
    class Iterator {
    private:
        KDTree* tree;
        Node* node;

        Iterator(KDTree* tree, Node* node) : tree(tree), node(node) {}

        /**
         * Increment the iterator
         * Time complexity: O(log n)
         */
        void increment() {
            // FIXME: implemented
            // TODO: edge cases
            if (node->right){ // Middle node
                if (node->right->left){ // right children has left children
                    node = node->right;
                    while (node->left){
                        node = node->left; // way down to the leftmost node
                    }
                    return;
                }
                else{ // right children has no left children
                    node = node->right;
                }
            }
            else if(!node->right){ // leaf node
                if (node->isLeftChild()){ // node is left children of its parent
                    node = node->parent;
                }
                else { // node is right children of its parent
                    if (node->isRightMostNode()){
                        node = nullptr;
                        return;
                    }
                    node = node->parent->parent;
                }
            }
        }

        /**
         * Decrement the iterator
         * Time complexity: O(log n)
         */
        void decrement() {
            // FIXME: implemented
            // TODO: edge cases
            if (node->left){
                if (node->left->right){
                    node = node->left;
                    while(node->right){
                        node = node->right;
                    }
                    return;
                }
                else{
                    node = node->left;
                }
            }
            else if (!node->left){
                if(node->isRightChild()){
                    node = node->parent;
                }
                else{
                    if (node->isLeftMostNode()){
                        node = nullptr;
                        return;
                    }
                    node = node->parent->parent;
                }
            }
        }

    public:
        friend class KDTree;

        Iterator() = delete;

        Iterator(const Iterator&) = default;

        Iterator& operator=(const Iterator&) = default;

        Iterator& operator++() {
            increment();
            return *this;
        }

        Iterator operator++(int) {
            Iterator temp = *this;
            increment();
            return temp;
        }

        Iterator& operator--() {
            decrement();
            return *this;
        }

        Iterator operator--(int) {
            Iterator temp = *this;
            decrement();
            return temp;
        }

        bool operator==(const Iterator& that) const {
            return node == that.node;
        }

        bool operator!=(const Iterator& that) const {
            return node != that.node;
        }

        Data* operator->() {
            return &(node->data);
        }

        Data& operator*() {
            return node->data;
        }
    };

protected:                      // DO NOT USE private HERE!
    Node* root = nullptr;       // root of the tree
    size_t treeSize = 0;        // size of  the tree

    /**
     * Find the node with key
     * Time Complexity: O(k log n)
     * @tparam DIM current dimension of node
     * @param key
     * @param node
     * @return the node with key, or nullptr if not found
     */
    template<size_t DIM>
    Node* find(const Key& key, Node* node) {
        //constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        // FIXME: implemented
        return findHelper<DIM>(node, key);
    }

    /**
     * Insert the key-value pair, if the key already exists, replace the value only
     * Time Complexity: O(k log n)
     * @tparam DIM current dimension of node
     * @param key
     * @param value
     * @param node
     * @param parent
     * @return whether insertion took place (return false if the key already exists)
     */
    template<size_t DIM>
    bool insert(const Key& key, const Value& value, Node*& node, Node* parent) {
        // constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        // FIXME: implemented
        return insertHelper<DIM>(node, key, value, parent);
    }

    /**
     * Compare two keys on a dimension
     * Time Complexity: O(1)
     * @tparam DIM comparison dimension
     * @tparam Compare
     * @param a
     * @param b
     * @param compare
     * @return relationship of two keys on a dimension with the compare function
     */
    template<size_t DIM, typename Compare>
    static bool compareKey(const Key &a, const Key &b, Compare compare = Compare()) {
        if (std::get<DIM>(a) != std::get<DIM>(b)){
            return compare(std::get<DIM>(a), std::get<DIM>(b));
        }
        return compare(a, b);
    }

    /**
     * Compare two nodes on a dimension
     * Time Complexity: O(1)
     * @tparam DIM comparison dimension
     * @tparam Compare
     * @param a
     * @param b
     * @param compare
     * @return the minimum / maximum of two nodes
     */
    template<size_t DIM, typename Compare>
    static Node* compareNode(Node* a, Node* b, Compare compare = Compare()) {
        if (!a) return b;
        if (!b) return a;
        return compareKey<DIM, std::less<>>(a->key(), b->key(), compare) ? a : b;
    }

    template<size_t DIM, typename Compare>
    static Node* compareNodeGt(Node* a, Node* b, Compare compare = Compare()) {
        if (!a) return b;
        if (!b) return a;
        return compareKey<DIM, std::greater<>>(a->key(), b->key(), compare) ? a : b;
    }

    /**
     * Find the minimum node on a dimension
     * Time Complexity: ?
     * @tparam DIM_CMP comparison dimension
     * @tparam DIM current dimension of node
     * @param node
     * @return the minimum node on a dimension
     */
    template<size_t DIM_CMP, size_t DIM>
    Node* findMin(Node* node) {
        //constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        // FIXME: implemented
        return findMinHelper<DIM_CMP, DIM>(node);
    }

    /**
     * Find the maximum node on a dimension
     * Time Complexity: ?
     * @tparam DIM_CMP comparison dimension
     * @tparam DIM current dimension of node
     * @param node
     * @return the maximum node on a dimension
     */
    template<size_t DIM_CMP, size_t DIM>
    Node* findMax(Node* node) {
        // constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        // FIXME: implemented
        return findMaxHelper<DIM_CMP, DIM>(node);
    }

    template<size_t DIM>
    Node* findMinDynamic(size_t dim) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (dim >= KeySize) {
            dim %= KeySize;
        }
        if (dim == DIM) return findMin<DIM, 0>(root);
        return findMinDynamic<DIM_NEXT>(dim);
    }

    template<size_t DIM>
    Node* findMaxDynamic(size_t dim) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (dim >= KeySize) {
            dim %= KeySize;
        }
        if (dim == DIM) return findMax<DIM, 0>(root);
        return findMaxDynamic<DIM_NEXT>(dim);
    }

    /**
     * Erase a node with key (check the pseudocode in project description)
     * Time Complexity: max{O(k log n), O(findMin)}
     * @tparam DIM current dimension of node
     * @param node
     * @param key
     * @return nullptr if node is erased, else the (probably) replaced node
     */
    template<size_t DIM>
    Node* erase(Node* node, const Key& key) {
        // constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        // FIXME: implemented
        return eraseHelper<DIM>(node, key);
    }

    template<size_t DIM>
    Node* eraseDynamic(Node* node, size_t dim) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (dim >= KeySize) {
            dim %= KeySize;
        }
        if (dim == DIM) return erase<DIM>(node, node->key());
        return eraseDynamic<DIM_NEXT>(node, dim);
    }

    template<size_t DIM>
    Node* findHelper(Node* thisNode, Key k){
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (!thisNode){
            return thisNode;
        }
        if (k==thisNode->key()){  // FIXME: std::tuple should have an overload for ==
            return thisNode;
        }
        if (compareKey<DIM, std::less<>>(k, thisNode->key())){ // k[dim] < thisNode->key()[dim]
            return findHelper<DIM_NEXT>(thisNode->left, k);
        }
        else{
            return findHelper<DIM_NEXT>(thisNode->right, k);
        }
    }

    template<size_t DIM>
    bool insertHelper(Node* &thisNode, Key k, Value v, Node* parent, int fromLeft=0){
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (!thisNode){
            thisNode = new Node(k, v, parent);
            if (fromLeft==1) { parent->left = thisNode; }
            else if (fromLeft==-1){ parent->right = thisNode; }
            treeSize++;
            return true;
        }
        if (k==thisNode->key()){
            thisNode->value()=v;
        }
        if (compareKey<DIM, std::less<>>(k, thisNode->key())){
            return insertHelper<DIM_NEXT>(thisNode->left, k, v, thisNode, 1);
        }
        else{
            return insertHelper<DIM_NEXT>(thisNode->right, k, v, thisNode, -1);
        }
    }

//    template<size_t DIM>
//    Node* minNode(Node* node1, Node* node2){
//        std::cout << ((node1!= nullptr)?node1->value():-999) << " and " << ((node2!=nullptr)?node2->value():-999) << " in DIM " << DIM << "\n";
//        if (!node1 && !node2) { return node1; }
//        if (!node1) { return node2; }
//        if (!node2) { return node1; }
//        if (compareNode<DIM, std::less<>>(node1, node2)) { return node1; }
//        else { return node2; }
//    }
//
//    template<size_t DIM>
//    Node* maxNode(Node* node1, Node* node2){
//        if (!node1 && !node2) { return node1; }
//        if (!node1) { return node2; }
//        if (!node2) { return node1; }
//        if (!compareNode<DIM, std::less<>>(node1, node2) && node1->key()!=node2->key()) { return node1; }
//        else { return node2; }
//    }

    template<size_t DIM_CMP, size_t DIM>
    Node* findMinHelper(Node *thisNode){
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (!thisNode) {
            return thisNode;
        }
        Node *min = findMinHelper<DIM_CMP, DIM_NEXT>(thisNode->left);
        if (DIM_CMP != DIM){
            Node* rightMin = findMinHelper<DIM_CMP, DIM_NEXT>(thisNode->right);
            min = compareNode<DIM_CMP,std::less<>>(min, rightMin);
        }
        return compareNode<DIM_CMP,std::less<>>(min, thisNode);
    }

    template<size_t DIM_CMP, size_t DIM>
    Node* findMaxHelper(Node* thisNode){
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (!thisNode) {
            return thisNode;
        }
        Node *max = findMaxHelper<DIM_CMP, DIM_NEXT>(thisNode->right);
        if (DIM_CMP != DIM){
            Node* leftMax = findMaxHelper<DIM_CMP, DIM_NEXT>(thisNode->left);
            max = compareNodeGt<DIM_CMP,std::greater<>>(leftMax, max);
        }
        return compareNodeGt<DIM_CMP,std::greater<>>(thisNode, max);
    }

    template<size_t DIM>
    Node* eraseHelper(Node* &thisNode, const Key k){
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (!thisNode) {
            return nullptr;
        }
        if (k == thisNode->key()){
            //std::cout << "Node " << thisNode->value() << " left: " << thisNode->hasLeftSubTree() << " right: " << thisNode->hasRightSubTree() << " isLeaf: " << thisNode->isLeafNode() << std::endl;
            if (thisNode->isLeafNode()) { // Node is a leaf
                deleteNodeHelper(thisNode);
                treeSize--;
                return nullptr;
            }
            else if (thisNode->hasRightSubTree()){
                Node *minNode = findMinHelper<DIM, DIM_NEXT>(thisNode->right);
                Node *left = thisNode->left;
                Node *right = thisNode->right;
                Node *parent = thisNode->parent;
                delete thisNode;
                thisNode = new Node(minNode->key(), minNode->value(), parent);
                thisNode->left = left;
                thisNode->right = eraseHelper<DIM_NEXT>(right, thisNode->key());
            }
            else if (thisNode->hasLeftSubTree()){
                Node *maxNode = findMaxHelper<DIM, DIM_NEXT>(thisNode->left);
                Node *left = thisNode->left;
                Node *right = thisNode->right;
                Node *parent = thisNode->parent;
                delete thisNode;
                thisNode = new Node(maxNode->key(), maxNode->value(), parent);
                thisNode->right = right;
                thisNode->left = eraseHelper<DIM_NEXT>(left, thisNode->key());
;            }
        }
        else{
            if (compareKey<DIM, std::less<>>(k, thisNode->key())){
                thisNode->left = eraseHelper<DIM_NEXT>(thisNode->left, k);
            }
            else{
                thisNode->right = eraseHelper<DIM_NEXT>(thisNode->right, k);
            }
        }
        return thisNode;
    }

    // Only used for leaf nodes
    void deleteNodeHelper(Node* thisNode){
        if (thisNode == thisNode->parent->left){
            thisNode->parent->left = nullptr;
        }
        else if (thisNode == thisNode->parent->right){
            thisNode->parent->right = nullptr;
        }
        delete thisNode;
    }

    template<size_t DIM>
    Node* constructHelper(Node* parent, inputVecIt beginIt, inputVecIt endIt){
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        size_t vecSize = (size_t)(endIt - beginIt);
        //std::cout << vecSize <<std::endl;
        if (vecSize == 0) {
            return nullptr;
        }
        vecSize = (vecSize / 2 * 2 == vecSize) ? vecSize / 2 - 1 : vecSize / 2;
        inputVecIt medianIt = beginIt + vecSize;
        //std::cout << beginIt->second << " " << medianIt->second << " " << (endIt-1)->second << " " << std::endl;
        std::nth_element(beginIt, medianIt, endIt, [](const auto &a, const auto &b){return compareKey<DIM, std::less<>>(a.first, b.first);}); // partition data into left, median and right
        Node* thisNode = new Node(medianIt->first, medianIt->second, parent);
        thisNode->left = constructHelper<DIM_NEXT>(thisNode, beginIt, medianIt);
        thisNode->right = constructHelper<DIM_NEXT>(thisNode, medianIt + 1, endIt);
        return thisNode;
    }

    Node* copyConstructHelper(Node* thisNode, Node* parentNode){
        Node* newNode = new Node(thisNode->key(), thisNode->value(), parentNode);
        if (thisNode->hasLeftSubTree()){
            newNode->left = copyConstructHelper(thisNode->left, thisNode);
        }
        if (thisNode->hasRightSubTree()){
            newNode->right = copyConstructHelper(thisNode->right, thisNode);
        }
        return thisNode;
    }

    void deleteHelper(Node* thisNode){
        if (thisNode->hasLeftSubTree()){
            deleteHelper(thisNode->left);
        }
        if (thisNode->hasRightSubTree()){
            deleteHelper(thisNode->right);
        }
        delete thisNode;
    }

    template<size_t DIM>
    static bool compareAllKeysHelper(const Data& a, const Data& b){
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        const Key keyA = a.first;
        const Key keyB = b.first;
        if (!compareKey<DIM, std::less<>>(keyA, keyB)){ // A<B
            return false;
        }
        else{
            if (DIM!=(KeySize-1)){
                return compareAllKeysHelper<DIM_NEXT>(a,b);
            }
            return true;
        }
    }

    template<size_t DIM>
    static bool compareAllKeysHelperGt(const Data& a, const Data& b){
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        const Key keyA = a.first;
        const Key keyB = b.first;
        if (!compareKey<DIM, std::greater<>>(keyA, keyB)){ // A<B
            return false;
        }
        else{
            if (DIM!=(KeySize-1)){
                return compareAllKeysHelper<DIM_NEXT>(a,b);
            }
            return true;
        }
    }

public:
    KDTree() = default;

    /**
     * Time complexity: O(kn log n)
     * @param v we pass by value here because v need to be modified
     */
    explicit KDTree(std::vector<std::pair<Key, Value>> v) {
        // TODO: implement this function
        std::stable_sort(v.begin(), v.end(), compareAllKeysHelper<0>);
        auto eraseIt = std::unique(v.rbegin(), v.rend());
        v.erase(v.begin(), eraseIt.base());
        root = constructHelper<0>(nullptr, v.begin(), v.end());
        treeSize = v.size();
    }

    /**
     * Time complexity: O(n)
     */
    KDTree(const KDTree& that) {
        // FIXME: implemented
        treeSize = that.treeSize;
        root = copyConstructHelper(that.root, nullptr);
    }

    /**
     * Time complexity: O(n)
     */
    KDTree& operator=(const KDTree& that) {
        // FIXME: implemented
        treeSize = that.treeSize;
        deleteNodeHelper(root);
        root = copyConstructHelper(that.root, nullptr);
        return *this;
    }

    /**
     * Time complexity: O(n)
     */
    ~KDTree() {
        // FIXME: implemented
        deleteHelper(root);
    }

    Iterator begin() {
        if (!root) return end();
        auto node = root;
        while (node->left) node = node->left;
        return Iterator(this, node);
    }

    Iterator end() {
        return Iterator(this, nullptr);
    }

    Iterator find(const Key& key) {
        return Iterator(this, find<0>(key, root));
    }

    void insert(const Key& key, const Value& value) {
        insert<0>(key, value, root, nullptr);
    }

    template<size_t DIM>
    Iterator findMin() {
        return Iterator(this, findMin<DIM, 0>(root));
    }

    Iterator findMin(size_t dim) {
        return Iterator(this, findMinDynamic<0>(dim));
    }

    template<size_t DIM>
    Iterator findMax() {
        return Iterator(this, findMax<DIM, 0>(root));
    }

    Iterator findMax(size_t dim) {
        return Iterator(this, findMaxDynamic<0>(dim));
    }

    bool erase(const Key& key) {
        auto prevSize = treeSize;
        erase<0>(root, key);
        return prevSize > treeSize;
    }

    Iterator erase(Iterator it) {
        if (it == end()) return it;
        auto node = it.node;
        if (!it.node->left && !it.node->right) {
            it.node = it.node->parent;
        }
        size_t depth = 0;
        auto temp = node->parent;
        while (temp) {
            temp = temp->parent;
            ++depth;
        }
        eraseDynamic<0>(node, depth % KeySize);
        return it;
    }

    size_t size() const { return treeSize; }
};

template<size_t SIZE, typename T>
auto vector2tuple(std::vector<T> &vec) {
    T t = T();
    std::vector<T> tempvec;
    tempvec.push_back(t);
    tempvec.pop_back();  // trick the stupid compiler
    if (!vec.empty()) {
        t = vec.back();
        vec.pop_back();
    }
    if constexpr (SIZE > 0) {
        return std::tuple_cat(vector2tuple<SIZE - 1, T>(vec),
                              std::make_tuple<T>(std::move(t)));
    } else {
        return std::make_tuple<>();
    }
}