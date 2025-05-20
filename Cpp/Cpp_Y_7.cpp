#include <iostream>
#include <vector>
using namespace std;

inline bool is_valid(int i, int n) {
    return 0 <= i && i < n;
}

#define ASSERT_VALID(cond) do { \
if (!(cond)) { \
cerr << "Invalid position" << endl; \
exit(1); \
} \
} while (0)

template<typename T> class LinearList {
public:
    virtual ~LinearList() = default;

    virtual T& get_element(int position) = 0;

    virtual void insert(int position, const T& elem) = 0;
};

template<typename T> class ArrayList : public LinearList<T> {
    vector<T> data;

public:
    T& get_element(int position) override {
        ASSERT_VALID(is_valid(position, data.size()));
        return data[position];
    }

    void insert(int position, const T& elem) override {
        ASSERT_VALID(is_valid(position, data.size() + 1));
        data.insert(data.begin() + position, elem);
    }
};

template<typename T> class LinkedList : public LinearList<T> {
    struct Node {
        T value;
        Node* next;

        Node(const T& value, Node* next = nullptr)
            : value(value), next(next) {}
    };

    Node* head;
    unsigned size;

public:
    LinkedList() : head(nullptr), size(0) {}

    ~LinkedList() override {
        while (head) {
            Node* temp = head;
            head       = head->next;
            delete temp;
        }
    }

    T& get_element(int position) override {
        ASSERT_VALID(is_valid(position, size));
        Node* curr = head;
        for (int i = 0; i < position; i++)
            curr   = curr->next;
        return curr->value;
    }

    void insert(int position, const T& elem) override {
        ASSERT_VALID(is_valid(position, size + 1));
        Node* newNode = new Node(elem);
        if (position == 0) {
            newNode->next = head;
            head          = newNode;
        } else {
            Node* previousNode = head;
            for (int i       = 0; i < position - 1; i++)
                previousNode = previousNode->next;
            newNode->next      = previousNode->next;
            previousNode->next = newNode;
        }
        ++size;
    }
};

template<typename T> T find(LinearList<T>& list, int position) {
    return list.get_element(position);
}

int main() {
    ArrayList<int> intArray;
    LinkedList<int> intLinked;
    intArray.insert(0, 10);
    intArray.insert(1, 20);
    intArray.insert(2, 30);
    intArray.insert(3, 40);
    intLinked.insert(0, 10);
    intLinked.insert(1, 20);
    intLinked.insert(2, 30);
    intLinked.insert(3, 40);
    cout << "ArrayList<int> element at 3rd position: " << find(intArray, 2) << endl;
    cout << "LinkedList<int> element at 3rd position: " << find(intLinked, 2) << endl;

    ArrayList<float> floatArray;
    LinkedList<float> floatLinked;
    floatArray.insert(0, 1.1f);
    floatArray.insert(1, 2.2f);
    floatArray.insert(2, 3.3f);
    floatArray.insert(3, 4.4f);
    floatLinked.insert(0, 1.1f);
    floatLinked.insert(1, 2.2f);
    floatLinked.insert(2, 3.3f);
    floatLinked.insert(3, 4.4f);
    cout << "ArrayList<float> element at 3rd position: " << find(floatArray, 2) << endl;
    cout << "LinkedList<float> element at 3rd position: " << find(floatLinked, 2) << endl;

    return 0;
}
