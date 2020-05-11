/*
    Dated: 12th May,2020
    RunTime:        64ms        faster  than    88.15%  of C++ online submissions
    Memory Used:    26.1 MB     less    than    100%    of C++ online submissions
    
*/
#include <fstream>
#include<cstdio>
#include<cstdlib>
#include<iostream>
#include<string>

using namespace std;

#define ALPHABET_SIZE 58
#define START_ALPHA 'A'

struct TrieNode;
struct DLLNode {
    DLLNode* prev;
    DLLNode* next;
    int val;
    int num_keys;
    TrieNode *keyListTail;
};

struct TrieNode {
    public:
    bool isEnd;
    TrieNode* children[ALPHABET_SIZE];
    TrieNode *parent;
    int parentIdx;

    TrieNode *nextListNode;     // Association to Keys With Same Val
    TrieNode *prevListNode;     // Association to Keys With Same Val

    DLLNode *valNode;           // Association to DLL Node
    int depth;

    TrieNode(){
        isEnd = false;
        for(int i=0;i<ALPHABET_SIZE;i++)
            children[i] = NULL;
        parent = NULL;
        parentIdx = -1;

        nextListNode = NULL;
        prevListNode = NULL;
        valNode = NULL;

        depth = 0;
    }
};
TrieNode* createTrieNode() {
    TrieNode *temp = (TrieNode*)malloc(sizeof(TrieNode));
    temp->isEnd = false;
    for(int i=0;i<ALPHABET_SIZE;i++)
        temp->children[i] = NULL;
    temp->parent = NULL;
    temp->parentIdx = -1;
    temp->depth = 0;
    temp->valNode = NULL;           // Association to DLL Node

    temp->nextListNode = NULL;      // Association to Keys With Same Val
    temp->prevListNode = NULL;      // Association to Keys With Same Val
    return temp;
};

void deleteTrieNode(TrieNode *temp) {
    temp->isEnd = false;
    for(int i=0;i<ALPHABET_SIZE;i++)
        temp->children[i] = NULL;
    temp->parent = NULL;
    temp->parentIdx = -1;
    temp->depth = 0;
    temp->valNode = NULL;           // Association to DLL Node
    if(temp->nextListNode)
        temp->nextListNode = NULL;      // Association to Keys With Same Val
    if(temp->prevListNode)
        temp->prevListNode = NULL;

    free(temp);
}

class Trie {
    public:
    TrieNode root;

    Trie() {
        root.isEnd = false;
        for(int i=0;i<ALPHABET_SIZE;i++)
            root.children[i] = NULL;
        root.parent = NULL;

        root.depth = 0;
        root.valNode = NULL;

        root.nextListNode = NULL;     // Association to Keys With Same Val
        root.prevListNode = NULL;     // Association to Keys With Same Val
        root.parentIdx = -1;
    }

    TrieNode* insert(const char *key) {
        TrieNode *ptr = &root;

        for(int i=0;key[i]!='\0';i++) {
            if(ptr->children[key[i]-START_ALPHA]==NULL) {
                ptr->children[key[i]-START_ALPHA] = createTrieNode(); // Creating New Node
                ptr->children[key[i]-START_ALPHA]->isEnd = false;
                ptr->children[key[i]-START_ALPHA]->parent = ptr;   // Assigning Parent
                ptr->children[key[i]-START_ALPHA]->depth = i+1;
                ptr->children[key[i]-START_ALPHA]->valNode = NULL;
                ptr->children[key[i]-START_ALPHA]->parentIdx = key[i]-START_ALPHA;
            }
            ptr = ptr->children[key[i]-START_ALPHA];
        }
        ptr->isEnd = true;
        return ptr;
    }

    TrieNode* getKeyNode(const char *key) {
        TrieNode *ptr = &root;
        for(int i=0;key[i]!='\0';i++) {
            if(ptr->children[key[i]-START_ALPHA]==NULL)
                return NULL;
            ptr = ptr->children[key[i]-START_ALPHA];
        }
        if(ptr->isEnd)
            return ptr;
        else
            return NULL;
    }

    bool search(const char *key) {
        TrieNode *ptr = &root;
        for(int i=0;key[i]!='\0';i++) {
            if(ptr->children[key[i]-START_ALPHA]==NULL)
                return false;
            ptr = ptr->children[key[i]-START_ALPHA];
        }
        if(ptr->isEnd)
            return true;
        else
            return false;
    }

    int removeKey(const char *key)
    {
        // 0 SUCCESS
        // 1 FAILURE
        TrieNode *ptr = &root;
        for(int i=0;key[i]!='\0';i++) {
            if(ptr->children[key[i]-START_ALPHA]==NULL)
                return 1; // Key Does Not Exist
            ptr = ptr->children[key[i]-START_ALPHA];
        }
        return removeKeyFromEnd(ptr);
    }

    int removeKeyFromEnd(TrieNode *ptr) {
        if(ptr->isEnd)
            ptr->isEnd = false;
        else
            return 1;

        int idx;

        while(ptr->parentIdx!=-1)
        {
            if(ptr->isEnd)
                return 0;

            for(int i=0;i<ALPHABET_SIZE;i++)
            {
                if(ptr->children[i]!=NULL)
                    return 0;
            }
            // All Other Children Are Null
            // So delete Node

            TrieNode *temp = ptr;
            idx = ptr->parentIdx;
            ptr = ptr->parent;
            temp->parent = NULL;
            ptr->children[idx] = NULL;
            deleteTrieNode(temp);
            // Set Parents Children idx to NULL
        }
        return 0;
    }
    string getKeyString(TrieNode *ptr) {
        string ans;
        int len = ptr->depth;
        ans.resize(len);
        ans[len--] = '\0';

        while(ptr->parentIdx!=-1)
        {
            ans[len--] = (char)(int(START_ALPHA) + ptr->parentIdx);
            ptr = ptr->parent;
        }
        return ans;
    }
};

class SortedDLL {
    public:
    DLLNode *head;
    DLLNode *tail;
    int num_nodes;

    SortedDLL() {
        head = NULL;
        tail = NULL;
        num_nodes = 0;
    }

    DLLNode* insertAtBeg(int val) {
        ++num_nodes;
        if(head==NULL)
        {
            // Adding First Node
            head=(DLLNode*)malloc(sizeof(DLLNode));
            head->val = val;     // val is 1 according to Problem
            head->prev = NULL;
            head->next = NULL;
            head->num_keys = 1;

            head->keyListTail = NULL;
            tail = head;
        }
        else
        {
            head->prev=(DLLNode*)malloc(sizeof(DLLNode));

            head->prev->val = val;     // val is 1 according to Problem
            head->prev->prev = NULL;
            head->prev->next = head;
            head->prev->num_keys = 1;


            head = head->prev;
            head->keyListTail = NULL;
        }
        return head;
    }
    // Not Used For This Problem
    DLLNode* insertAtEnd(int val) {
        ++num_nodes;
        if(tail==NULL)
        {
            // Adding First Node
            head=(DLLNode*)malloc(sizeof(DLLNode));
            head->val = val;     // val is 1 according to Problem
            head->prev = NULL;
            head->next = NULL;
            head->num_keys = 1;
            head->keyListTail = NULL;
            tail = head;
        }
        else
        {
            tail->next=(DLLNode*)malloc(sizeof(DLLNode));

            tail->next->val = 1;     // val is 1 according to Problem
            tail->next->prev = tail;
            tail->next->next = NULL;
            tail->next->num_keys = 1;

            tail = tail->next;
            tail->keyListTail = NULL;

        }
        return tail;
    }
    DLLNode* insertAfter(DLLNode *ptr, int val) {
        ++num_nodes;
        DLLNode* temp = (DLLNode*)malloc(sizeof(DLLNode));
        temp->next = ptr->next;
        temp->prev = ptr;
        temp->val = val;
        temp->num_keys = 1;
        temp->keyListTail = NULL;
        if(ptr->next)
            ptr->next->prev = temp;
        ptr->next = temp;

        if(tail==ptr)
            tail = ptr->next;

        return ptr->next;
    }
    DLLNode* insertBefore(DLLNode *ptr, int val) {
        num_nodes++;

        DLLNode* temp = (DLLNode*)malloc(sizeof(DLLNode));
        temp->prev = ptr->prev;
        temp->next = ptr;
        temp->val = val;
        temp->num_keys = 1;
        temp->keyListTail = NULL;
        if(ptr->prev)
            ptr->prev->next = temp;

        ptr->prev = temp;

        if(head==ptr)
            head = ptr->prev;

        return ptr->prev;
    }
    void remove(DLLNode *ptr) {
        --num_nodes;
        if(head==tail)
        {
            ptr->next = ptr->prev = NULL;
            head = tail = NULL;
            free(ptr);
            return;
        }
        if(ptr!=tail)
            ptr->next->prev = ptr->prev;
        else
            tail = ptr->prev;

        if(ptr!=head)
            ptr->prev->next = ptr->next;
        else
            head = ptr->next;
        ptr->next = NULL;
        ptr->prev = NULL;
        free(ptr);
    }
};

class AllOne {
public:
    Trie keyDB;
    SortedDLL valDB;
    /** Initialize your data structure here. */
    AllOne() {
        valDB.head = NULL;
        valDB.tail = NULL;

        keyDB.root.depth = 0;
        keyDB.root.valNode = NULL;
    }

    /** Inserts a new key <Key> with value 1. Or increments an existing key by 1. */
    void inc(string key) {
        TrieNode *ptr = keyDB.insert(key.c_str());
        DLLNode* valNode = ptr->valNode;
        if(valNode==NULL)
        {
            if(valDB.head && valDB.head->val==1)
            {
                valNode = valDB.head;
                ++(valNode->num_keys);
            }
            else
                valNode = valDB.insertAtBeg(1);
        }
        else
        {
            int currentVal = valNode->val;

            if(valNode->next && valNode->next->val == currentVal + 1)
            {
                DLLNode *oldvalNode = valNode;

                valNode = valNode->next;

                --(oldvalNode->num_keys);

                if(ptr==oldvalNode->keyListTail)
                    oldvalNode->keyListTail = ptr->prevListNode;
                // Remove Node if no keys Left for that val
                if(oldvalNode->num_keys==0)
                    valDB.remove(oldvalNode);
                ++(valNode->num_keys);
            }
            else
            {
                DLLNode *oldvalNode = valNode;
                --(oldvalNode->num_keys);
                // Remove Node if no keys Left for that val
                if(ptr==oldvalNode->keyListTail)
                    oldvalNode->keyListTail = ptr->prevListNode;

                valNode = valDB.insertAfter(valNode, currentVal+1);

                if(oldvalNode->num_keys==0)
                    valDB.remove(oldvalNode);
            }
        }
        ptr->valNode = valNode;
        if(ptr->nextListNode)
            ptr->nextListNode->prevListNode = ptr->prevListNode;
        if(ptr->prevListNode)
            ptr->prevListNode->nextListNode = ptr->nextListNode;

        ptr->nextListNode = NULL;
        ptr->prevListNode = NULL;

        ptr->prevListNode = valNode->keyListTail;
        if(valNode->keyListTail)
            valNode->keyListTail->nextListNode = ptr;
        valNode->keyListTail = ptr;
    }

    /** Decrements an existing key by 1. If Key's value is 1, remove it from the data structure. */
    void dec(string key) {
        TrieNode *ptr = keyDB.getKeyNode(key.c_str());
        if(!ptr)
            return;

        DLLNode* valNode = ptr->valNode;
        int currentVal = valNode->val;

        if(valNode->val==1)
        {
            --(valNode->num_keys);

            ptr->valNode= NULL;

            if(ptr==valNode->keyListTail)
                valNode->keyListTail = valNode->keyListTail->prevListNode;

            if(valNode->num_keys==0)
                valDB.remove(valNode);


            valNode = NULL;
        }
        else if(valNode->prev && (valNode->prev->val == currentVal - 1))
        {
            DLLNode *oldvalNode = valNode;

            valNode = valNode->prev;

            --(oldvalNode->num_keys);
            if(ptr==oldvalNode->keyListTail)
                oldvalNode->keyListTail = ptr->prevListNode;

            // Remove Node if no keys Left for that val
            if(oldvalNode->num_keys==0)
                valDB.remove(oldvalNode);
            ++(valNode->num_keys);
        }
        else
        {
            DLLNode *oldvalNode = valNode;
            --(oldvalNode->num_keys);

            valNode = valDB.insertBefore(valNode, currentVal-1);

            if(ptr==oldvalNode->keyListTail)
                oldvalNode->keyListTail = ptr->prevListNode;
            // Remove Node if no keys Left for that val
            if(oldvalNode->num_keys==0)
                valDB.remove(oldvalNode);
        }
        ptr->valNode = valNode;

        if(ptr->nextListNode)
            ptr->nextListNode->prevListNode = ptr->prevListNode;
        if(ptr->prevListNode)
            ptr->prevListNode->nextListNode = ptr->nextListNode;

        ptr->nextListNode = NULL;
        ptr->prevListNode = NULL;

        if(valNode)
            ptr->prevListNode = valNode->keyListTail;
        if(valNode)
        {
            if(valNode->keyListTail)
                valNode->keyListTail->nextListNode = ptr;
            valNode->keyListTail = ptr;
        }

        if(!valNode)
            keyDB.removeKeyFromEnd(ptr);
    }

    /** Returns one of the keys with maximal value. */
    string getMaxKey() {
        if(valDB.tail)
            return keyDB.getKeyString(valDB.tail->keyListTail);
        else
            return "";
    }

    /** Returns one of the keys with Minimal value. */
    string getMinKey() {
        if(valDB.head)
            return keyDB.getKeyString(valDB.head->keyListTail);
        else
            return "";
    }
};

/**
 * Your AllOne object will be instantiated and called as such:
 * AllOne* obj = new AllOne();
 * obj->inc(key);
 * obj->dec(key);
 * string param_3 = obj->getMaxKey();
 * string param_4 = obj->getMinKey();
 */
 
 
// Driver Code START
// For Debug Purpose
/*
Format:
5                                   // Num Of Operations
0 leet                              // 0 is inc operation
0 code                              // 0 is inc operation
1 leet                              // 1 is dec operation
2                                   // 2 is getMinKey operation
3                                   // 3 is getMaxKey operation

*/

 int main()
 {
    int t;
    int op;
    
    // reading from input.txt
    ifstream in("input.txt");
    streambuf *cinbuf = cin.rdbuf();
    cin.rdbuf(in.rdbuf());
    
    /** backup cout buffer and redirect to out.txt **/
    ofstream out("output.txt");
    streambuf *coutbuf = cout.rdbuf();
    cout.rdbuf(out.rdbuf());

    cin>>t;
    AllOne db;
    while(t--)
    {
        cin>>op;
        string str;
        switch(op)
        {
            case 0: cin>>str;
                    db.inc(str);
                    break;
            case 1: cin>>str;
                    db.dec(str);
                    break;
            case 2: str = db.getMinKey();
                    cout<<"Min Key : "<<str<<" Value: "<<db.valDB.head->val<<endl;
                    break;
            case 3: str = db.getMaxKey();
                    cout<<"Max Key : "<<str<<" Value: "<<db.valDB.tail->val<<endl;
                    break;
        }
    }
    cin.rdbuf(cinbuf);
    cout.rdbuf(coutbuf);
    return 0;
 }
