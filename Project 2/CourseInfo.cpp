//============================================================================
// Name        : CourseInfo.cpp
// Author      : Christopher Fusaro
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU CODE
// Description : Course Info in C++, Ansi-style
//============================================================================

#include <iostream>
#include <time.h>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================


// Structure to hold course information
struct Course {
    string courseId; // unique identifier
    string title;
    vector<string> prerequisites;
};

// Internal structure for tree node
struct Node {
    Course course;
    Node* left;
    Node* right;

    // Default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // Initialize with a course
    Node(Course aCourse) :
        Node() {
        course = aCourse;
    }
};

//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree {

private:
    Node* root;

    void addNode(Node* node, Course course);
    void inOrder(Node* node);
    Node* removeNode(Node* node, string courseId);

public:
    BinarySearchTree();
    void deleteSubtree(Node* node);
    virtual ~BinarySearchTree();
    void InOrder();
    void Insert(Course course);
    void insertNode(Node* node, Node* newNode);
    void Remove(string bidId);
    Course Search(string bidId);
    void PostOrder();
    void postOrder(Node* node);
    void PreOrder();
    void preOrder(Node* node);
    Node* findMinNode(Node* node);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
    // Initialize housekeeping variables
    // Root is equal to nullptr
    root = nullptr;
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
    // recurse from root deleting every node
    deleteSubtree(root);
}

void BinarySearchTree::deleteSubtree(Node* node) {
    if (node != nullptr) {
        // Recursively delete the left and right subtrees
        deleteSubtree(node->left);
        deleteSubtree(node->right);

        // Delete the current node itself
        delete node;
    }
}

void BinarySearchTree::InOrder() {
    // Start the in-order traversal from the root node
    inOrder(root);
}

void BinarySearchTree::PostOrder() {
    // Start the post-order traversal from the root node
    postOrder(root);
}

void BinarySearchTree::PreOrder() {
    // Start the pre-order traversal from the root node
    preOrder(root);
}

void BinarySearchTree::Insert(Course course) {
    // Create a new node to hold the course
    Node* newNode = new Node(course);

    if (root == nullptr) {
        // Tree is empty, so the new node becomes the root
        root = newNode;
    }
    else {
        // Insert the node recursively, maintaining BST property
        insertNode(root, newNode);
    }
}

void BinarySearchTree::insertNode(Node* node, Node* newNode) {
    if (newNode->course.courseId < node->course.courseId) {
        // New node should go to the left subtree
        if (node->left == nullptr) {
            node->left = newNode;
        }
        else {
            insertNode(node->left, newNode);
        }
    }
    else {
        // New node should go to the right subtree
        if (node->right == nullptr) {
            node->right = newNode;
        }
        else {
            insertNode(node->right, newNode);
        }
    }
}

void BinarySearchTree::Remove(string bidId) {
    root = removeNode(root, bidId);
}

Node* BinarySearchTree::removeNode(Node* node, string courseId) {
    if (node == nullptr) {
        // Node not found
        return nullptr;
    }

    if (courseId < node->course.courseId) {
        // Search in the left subtree
        node->left = removeNode(node->left, courseId);
    }
    else if (courseId > node->course.courseId) {
        // Search in the right subtree
        node->right = removeNode(node->right, courseId);
    }
    else {
        // Node to be deleted found

        // Case 1: No children
        if (node->left == nullptr && node->right == nullptr) {
            delete node;
            node = nullptr;
        }
        // Case 2: One child
        else if (node->left == nullptr) {
            Node* temp = node;
            node = node->right;
            delete temp;
        }
        else if (node->right == nullptr) {
            Node* temp = node;
            node = node->left;
            delete temp;
        }

        // Case 3: Two children
        else {
            // Find in-order successor (smallest in right subtree)
            Node* successor = findMinNode(node->right);

            // Replace the node's value with successor's value
            node->course = successor->course;

            // Recursively remove the successor (which is now a duplicate)
            node->right = removeNode(node->right, successor->course.courseId);
        }
    }
    return node;
}

Node* BinarySearchTree::findMinNode(Node* node) {
    while (node->left != nullptr) {
        node = node->left;
    }
    return node;
}

Course BinarySearchTree::Search(string courseId) {
    Node* currentNode = root;

    while (currentNode != nullptr) {
        if (courseId == currentNode->course.courseId) {
            // Bid found, return it
            return currentNode->course;
        }
        else if (courseId < currentNode->course.courseId) {
            // Search in the left subtree
            currentNode = currentNode->left;
        }
        else {
            // Search in the right subtree
            currentNode = currentNode->right;
        }
    }

    // Course not found, return an empty course object
    Course course;
    return course;
}

void BinarySearchTree::addNode(Node* node, Course course) {
    if (course.courseId < node->course.courseId) {
        // Add to the left subtree
        if (node->left == nullptr) {
            // Create a new node and insert it
            node->left = new Node(course);
        }
        else {
            // Recurse down the left subtree
            addNode(node->left, course);
        }
    }
    else {
        // Add to the right subtree
        if (node->right == nullptr) {
            // Create a new node and insert it
            node->right = new Node(course);
        }
        else {
            // Recurse down the right subtree
            addNode(node->right, course);
        }
    }
}

void BinarySearchTree::inOrder(Node* node) {
    if (node != nullptr) {
        // Recursively traverse the left subtree
        inOrder(node->left);

        // Process the current node (print its course ID and Title)
        cout << node->course.courseId << ", ";
        cout << node->course.title;
        cout << endl;
        
        

        // Recursively traverse the right subtree
        inOrder(node->right);
    }
}

void BinarySearchTree::postOrder(Node* node) {
    if (node != nullptr) {
        // Recursively traverse the left subtree
        postOrder(node->left);

        // Recursively traverse the right subtree
        postOrder(node->right);

        // Process the current node (print its course details)
        cout << "Course ID: " << node->course.courseId << endl;
        cout << "Title: " << node->course.title << endl;
        cout << endl;
    }
}

void BinarySearchTree::preOrder(Node* node) {
    if (node != nullptr) {
        // Process the current node (print its course details)s
        cout << "Course ID: " << node->course.courseId << endl;
        cout << "Title: " << node->course.title << endl;
        cout << endl;

        // Recursively traverse the left subtree
        preOrder(node->left);

        // Recursively traverse the right subtree
        preOrder(node->right);
    }
}
//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the bid information to the console (std::out)
 *
 * @param bid struct containing the bid info
 */
void displayCourse(Course course) {
    cout << course.courseId << ": " << course.title <<  endl;
    cout << "Prerequisites: ";
    for (unsigned int i = 0; i < course.prerequisites.size(); i++) {
        cout << course.prerequisites[i];
        if (i + 1 < course.prerequisites.size()) {
            cout << ", ";
        }
    }
    cout << endl;
    return;
}

/**
 * Load a CSV file containing courses into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the courses read
 */
void loadCourses(string csvPath, BinarySearchTree* bst) {
    cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    // read and display header row - optional
    vector<string> header = file.getHeader();
    for (auto const& c : header) {
        cout << c << " | ";
    }
    cout << "" << endl;

    try {
        // loop to read rows of a CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {

            // Create a data structure and add to the collection of courses
            Course course;
            course.courseId = file[i][0];
            course.title = file[i][1];
            if (file[i][2] != "") {
                course.prerequisites.push_back(file[i][2]);
                if (file[i][3] != "") {
                    course.prerequisites.push_back(file[i][3]);
                }
            }

            // push this course to the end
            bst->Insert(course);
        }
    }
    catch (csv::Error& e) {
        std::cerr << e.what() << std::endl;
    }
}

/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, courseKey;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        courseKey = "CSCI400";
        break;
    case 3:
        csvPath = argv[1];
        courseKey = argv[2];
        break;
    default:
        csvPath = "course.csv";
        courseKey = "CSCI400";
    }



    // Define a binary search tree to hold all courses
    BinarySearchTree* bst;
    bst = new BinarySearchTree();
    Course course;

    cout << "Please provide data file name (ex. course.csv) : ";
    cin >> csvPath;

    int choice = 0;
    while (choice != 9) {
        cout << "Welcome to the course planner." << endl;
        cout << endl;
        cout << "  1. Load Data Structure." << endl;
        cout << "  2. Print Course List." << endl;
        cout << "  3. Print Course." << endl;
        cout << "  9. Exit" << endl;
        cout << endl;
        cout << "What would you like to do? ";
        cin >> choice;

        switch (choice) {

        case 1:
            // Method call to load the course
            loadCourses(csvPath, bst);
            break;

        case 2:
            cout << "Here is a sample schedule:" << endl;
            cout << endl;
            bst->InOrder();
            cout << endl;
            break;

        case 3:
            cout << "What course do you want to know about? ";
            cin >> courseKey;
            course = bst->Search(courseKey);

            if (!course.courseId.empty()) {
                displayCourse(course);
            }
            else {
                cout << "Course Id " << courseKey << " not found." << endl;
            }
            break;

        case 4:
            cout << "Thank you for using the course planner!" << endl;
            break;

        default:
            cout << choice << " is not a valid option." << endl;
            break;

        }
    }

    cout << "Good bye." << endl;

    return 0;
}
