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

        bool isLeafNode() {return !left && !right;}

        bool isLeftChild() {return parent && this == this->parent->left;}

        bool isRightChild() {return parent && this == this->parent->right;}

        bool hasLeftSubTree() {return this->left;}

        bool hasRightSubTree() {return this->right;}
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
        return findHelper<DIM>(root, key);
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
        return insertHelper(node, key, value, parent);
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
    static bool compareKey(const Key& a, const Key& b, Compare compare = Compare()) {
        return compare(std::get<DIM>(a), std::get<DIM>(b));
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
        return findMinHelper<DIM>(node, DIM_CMP);
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
        return findMaxHelper<DIM>(node, DIM_CMP);
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
        return eraseHelper<DIM>(node, key, 0);
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
    bool insertHelper(Node* &thisNode, Key k, Value v, Node* parent){
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (!thisNode){
            thisNode = new Node(k, v, parent);
            return true;
        }
        if (k==thisNode->key()){
            thisNode->value()=v;
        }
        if (compareKey<DIM, std::less<>>(k, thisNode->key())){
            return insertHelper<DIM_NEXT>(thisNode->left, k, v, thisNode);
        }
        else{
            return insertHelper<DIM_NEXT>(thisNode->right, k, v, thisNode);
        }
    }

    template<size_t DIM>
    Node* findMinHelper(Node *thisNode, size_t dimCmp){
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (!thisNode) {
            return thisNode;
        }
        Node *min = findMinHelper<DIM_NEXT>(thisNode->left, dimCmp);
        if (dimCmp != DIM){
            Node* rightMin = findMinHelper<DIM_NEXT>(thisNode->right, dimCmp);
            if (!compareNode<dimCmp>(min, rightMin)){
                min = rightMin;
            }
        }
        if (compareNode<dimCmp>(min, thisNode)){
            return min;
        }
        else {
            return thisNode;
        }
    }

    template<size_t DIM>
    Node* findMaxHelper(Node* thisNode, size_t dimCmp){
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (!thisNode) {
            return thisNode;
        }
        Node *max = findMaxHelper<DIM_NEXT>(thisNode->right, dimCmp);
        if (dimCmp != DIM){
            Node* leftMax = findMaxHelper<DIM_NEXT>(thisNode->left, dimCmp);
            if (compareNode<dimCmp>(max, leftMax)){
                max = leftMax;
            }
        }
        if (!compareNode<dimCmp>(max, thisNode)){
            return max;
        }
        else {
            return thisNode;
        }
    }

    template<size_t DIM>
    Node* eraseHelper(Node* thisNode, Key k){
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (k == thisNode->key()){
            if (thisNode->isLeafNode()) { // Node is a leaf
                deleteNodeHelper(thisNode);
                return nullptr;
            }
            else if (thisNode->hasRightSubTree()){
                Node *minNode = findMinHelper<DIM>(thisNode->right);
                thisNode->key() = minNode->key();
                thisNode->value() = minNode->value();
                thisNode->right = eraseHelper<DIM_NEXT>(thisNode->right, minNode->key());
            }
            else if (thisNode->hasLeftSubTree()){
                Node* maxNode = findMaxHelper<DIM_NEXT>(thisNode->left);
                thisNode->key() = maxNode->key();
                thisNode->value() = maxNode->value();
                thisNode->left = eraseHelper<DIM_NEXT>(maxNode->left, maxNode->key());
;            }
        }
        else{
            if (compareKey<DIM, std::less<>>(k, thisNode->key())){
                thisNode->left = eraseHelper<DIM_NEXT>(thisNode->left, thisNode->key());
            }
            else{
                thisNode->right = eraseHelper<DIM_NEXT>(thisNode->right, thisNode->key());
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
        if (vecSize == 0){
            return nullptr;
        }
        vecSize /= 2;
        inputVecIt medianIt = beginIt + vecSize;
        std::cout << beginIt->second << " " << medianIt->second << " " << (endIt-1)->second << " " << std::endl;
        std::nth_element(beginIt, medianIt, endIt, compareAllKeysHelper<DIM>); // partition data into left, median and right
        Node* thisNode = new Node(medianIt->first, medianIt->second, parent);
        thisNode->left = constructHelper<DIM_NEXT>(thisNode, beginIt, medianIt - 1);
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
            deleteNodeHelper(thisNode->right);
        }
        delete thisNode;
    }

    template<size_t DIM>
    static bool compareAllKeysHelper(const Data& a, const Data& b){
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        const Key keyA = a.first;
        const Key keyB = b.first;
        if (compareKey<DIM, std::less<>>(keyA, keyB)){ // A<B
            return true;
        }
        else{
            if (DIM!=(KeySize-1)){
                return compareAllKeysHelper<DIM_NEXT>(a,b);
            }
            return false;
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
        for (auto &i :v){
            std::cout << i.second << " ";
        }
        std::cout << "\n";
        root = constructHelper<0>(nullptr, v.begin(), v.end());
        treeSize = v.size();
    }

    /**
     * Time complexity: O(n)
     */
    KDTree(const KDTree& that) {
        // FIXME: implemented
        treeSize = that.treeSize;
        root = copyConstructHelper(&(that.root), nullptr);
    }

    /**
     * Time complexity: O(n)
     */
    KDTree& operator=(const KDTree& that) {
        // FIXME: implemented
        treeSize = that.treeSize;
        deleteNodeHelper(root);
        root = copyConstructHelper(&(that.root), nullptr);
        return *this;
    }

    /**
     * Time complexity: O(n)
     */
    ~KDTree() {
        // FIXME: implemented
        deleteNodeHelper(root);
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