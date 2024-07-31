# QuadTree Image Compression and Decompression

## Overview

This C++ program utilizes a QuadTree data structure to perform image compression and decompression. The code reads an image from a file, compresses it using a QuadTree, and then writes various results to output files.

## Classes and Methods

### `QtNode` Class

The `QtNode` class represents a node in the QuadTree. Each node contains:

- `color`: The color of the node (0 or 1 for binary images).
- `upperR`: The upper row index of the node in the image.
- `upperC`: The upper column index of the node in the image.
- `size`: The size of the node (i.e., the width and height).
- `NWkid`, `NEkid`, `SWkid`, `SEkid`: Pointers to the four child nodes (North-West, North-East, South-West, South-East).

**Constructor:**

- `QtNode(int color, int upperR, int upperC, int size, QtNode * NWkid, QtNode * NEkid, QtNode * SWkid, QtNode * SEkid)`

**Methods:**

- `void printQtNode(QtNode * node, ofstream & outFile)`: Prints the details of the node to an output file.

### `QuadTree` Class

The `QuadTree` class manages the construction and traversal of the QuadTree.

**Attributes:**

- `QtNode * QtRoot`: Root of the QuadTree.
- `int numRows`, `numCols`: Number of rows and columns in the original image.
- `int minVal`, `maxVal`: Minimum and maximum pixel values.
- `int power2Size`: Size of the image padded to the next power of 2.
- `int ** imgAry`, `int ** newimgAry`: Arrays for the original and processed images.

**Constructor:**

- `QuadTree(int numRows, int numCols, int minVal, int maxVal)`: Initializes the QuadTree with image dimensions and value ranges.

**Methods:**

- `int computePower2(int r, int c)`: Computes the next power of 2 greater than the maximum of rows and columns.
- `void loadImage(ifstream & inFile, int ** imgAry)`: Loads the image from a file into `imgAry`.
- `QtNode * buildQuadTree(int ** Ary, int upR, int upC, int s, ofstream & outFile)`: Recursively builds the QuadTree from `Ary`.
- `int addKidsColor(QtNode & node)`: Adds the colors of the child nodes.
- `bool isLeaf(QtNode * node)`: Checks if a node is a leaf (i.e., has no children).
- `void preOrder(QtNode * Qt, ofstream & outFile)`: Performs a pre-order traversal of the QuadTree.
- `void postOrder(QtNode * Qt, ofstream & outFile)`: Performs a post-order traversal of the QuadTree.
- `void getLeaf(QtNode * Qt, int ** ImgAry)`: Retrieves leaf nodes and reconstructs the image in `ImgAry`.
- `void fillnewImgAry(QtNode * Qt, int ** ImgAry)`: Fills `ImgAry` with pixel values based on the QuadTree.
- `void zero2DAry(int ** Ary)`: Initializes a 2D array with zeros.

### `main` Function

The `main` function orchestrates the program:

1. **File Operations:**
   - Opens input and output files.
   - Reads image dimensions and value ranges.

2. **QuadTree Operations:**
   - Initializes the `QuadTree`.
   - Loads the image data.
   - Builds the QuadTree from the image data.
   - Performs pre-order and post-order traversals, writing results to output files.
   - Reconstructs the image from the QuadTree and writes it to an output file.

3. **File Closing:**
   - Closes all opened files.
