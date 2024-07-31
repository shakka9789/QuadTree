#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
using namespace std;

class QtNode {
  public:
    int color, upperR, upperC, size;
  QtNode * NWkid = NULL;
  QtNode * NEkid = NULL;
  QtNode * SWkid = NULL;
  QtNode * SEkid = NULL;

  QtNode(int color, int upperR, int upperC, int size, QtNode * NWkid, QtNode * NEkid, QtNode * SWkid, QtNode * SEkid) {
    this -> color = color;
    this -> upperR = upperR;
    this -> upperC = upperC;
    this -> size = size;

    this -> NWkid = NWkid;
    this -> NEkid = NEkid;
    this -> SWkid = SWkid;
    this -> SEkid = SEkid;
  }

  void printQtNode(QtNode * node, ofstream & outFile) {
    outFile << "Color: " << node -> color; //Easier to read format
    if (node -> upperR < 10) {
      outFile << " , upperR: " << node -> upperR << " ";
    } else {
      outFile << " , upperR: " << node -> upperR;
    }
    if (node -> upperC < 10) {
      outFile << " , upperC: " << node -> upperC << " ";
    } else {
      outFile << " , upperC: " << node -> upperC;
    }
    if (node -> size < 10) {
      outFile << ", Size: " << node -> size << " , ";
    } else {
      outFile << ", Size: " << node -> size << ", ";
    } //Easier to read format

    if (node -> NWkid != NULL) //Conditions incase one of the children is null
    {
      outFile << "NWColor: " << node -> NWkid -> color << "   , ";
    } else {
      outFile << "NWColor: null, ";
    }

    if (node -> NEkid != NULL) {
      outFile << "NEColor: " << node -> NEkid -> color << "   , ";
    } else {
      outFile << "NEColor: null, ";
    }

    if (node -> SWkid != NULL) {
      outFile << "SWColor: " << node -> SWkid -> color << "   , ";
    } else {
      outFile << "SWColor: null, ";
    }

    if (node -> SEkid != NULL) {
      outFile << "SEColor: " << node -> SEkid -> color << endl;
    } else {
      outFile << "SEColor: null" << endl;
    }

  }
};

class QuadTree {
  public:
    QtNode * QtRoot;
  int numRows, numCols, minVal, maxVal, power2Size;
  int ** imgAry;
  int ** newimgAry;

  QuadTree(int numRows, int numCols, int minVal, int maxVal) {
    this -> numRows = numRows;
    this -> numCols = numCols;
    this -> minVal = minVal;
    this -> maxVal = maxVal;

    power2Size = computePower2(numRows, numCols);

    imgAry = new int * [power2Size];
    for (int i = 0; i < power2Size; i++) {
      imgAry[i] = new int[power2Size];
    }

    newimgAry = new int * [power2Size];
    for (int i = 0; i < power2Size; i++) {
      newimgAry[i] = new int[power2Size];
    }
  }

  int computePower2(int r, int c) {
    int size = max(r, c);
    int power2 = 2;

    while (size > power2) {
      power2 *= 2;
    }

    return power2;
  }

  void loadImage(ifstream & inFile, int ** imgAry) {
    int r = 0;
    while (r < numRows) {
      int c = 0;
      while (c < numCols) {
        int data;
        inFile >> data;
        imgAry[r][c] = data;
        c++;
      }
      r++;
    }

  }

  QtNode * buildQuadTree(int ** Ary, int upR, int upC, int s, ofstream & outFile) {
    QtNode * newQtNode = new QtNode(-1, upR, upC, s, NULL, NULL, NULL, NULL);

    if (s == 1) {
      newQtNode -> color = Ary[upR][upC];
    } else {
      int halfSize = s / 2;

      newQtNode -> NWkid = buildQuadTree(Ary, upR, upC, halfSize, outFile);
      newQtNode -> NEkid = buildQuadTree(Ary, upR, upC + halfSize, halfSize, outFile);
      newQtNode -> SWkid = buildQuadTree(Ary, upR + halfSize, upC, halfSize, outFile);
      newQtNode -> SEkid = buildQuadTree(Ary, upR + halfSize, upC + halfSize, halfSize, outFile);

      int sumColor = addKidsColor( * newQtNode);

      if (sumColor == 0) {
        newQtNode -> color = 0;
        newQtNode -> NWkid = NULL;
        newQtNode -> NEkid = NULL;
        newQtNode -> SWkid = NULL;
        newQtNode -> SEkid = NULL;
      } else if (sumColor == 4) {
        newQtNode -> color = 1;
        newQtNode -> NWkid = NULL;
        newQtNode -> NEkid = NULL;
        newQtNode -> SWkid = NULL;
        newQtNode -> SEkid = NULL;
      } else {
        newQtNode -> color = 5;
      }
    }
    newQtNode -> printQtNode(newQtNode, outFile);
    return newQtNode;
  }

  int addKidsColor(QtNode & node) {
    int sum = node.NWkid -> color + node.NEkid -> color + node.SWkid -> color + node.SEkid -> color;
    return sum;
  }

  bool isLeaf(QtNode * node) {
    if (node -> NWkid == NULL && node -> NEkid == NULL && node -> SWkid == NULL && node -> SEkid == NULL) {
      return true;
    } else {
      return false;
    }
  }

  void preOrder(QtNode * Qt, ofstream & outFile) {
    if (isLeaf(Qt)) {
      Qt -> printQtNode(Qt, outFile);
    } else {
      Qt -> printQtNode(Qt, outFile);
      preOrder(Qt -> NWkid, outFile);
      preOrder(Qt -> NEkid, outFile);
      preOrder(Qt -> SWkid, outFile);
      preOrder(Qt -> SEkid, outFile);
    }
  }

  void postOrder(QtNode * Qt, ofstream & outFile) {
    if (isLeaf(Qt)) {
      Qt -> printQtNode(Qt, outFile);
    } else {
      preOrder(Qt -> NWkid, outFile);
      preOrder(Qt -> NEkid, outFile);
      preOrder(Qt -> SWkid, outFile);
      preOrder(Qt -> SEkid, outFile);
      Qt -> printQtNode(Qt, outFile);
    }
  }

  void getLeaf(QtNode * Qt, int ** ImgAry) {
    if (isLeaf(Qt)) {
      fillnewImgAry(Qt, ImgAry);
    } else {
      getLeaf(Qt -> NWkid, ImgAry);
      getLeaf(Qt -> NEkid, ImgAry);
      getLeaf(Qt -> SWkid, ImgAry);
      getLeaf(Qt -> SEkid, ImgAry);
    }
  }

  void fillnewImgAry(QtNode * Qt, int ** ImgAry) {
    int color, R, C, sz;

    color = Qt -> color;
    R = Qt -> upperR;
    C = Qt -> upperC;
    sz = Qt -> size;

    int i = R;

    while (i < R + sz) {
      int j = C;

      while (j < C + sz) {
        ImgAry[i][j] = color;
        j++;
      }

      i++;
    }
  }

  void zero2DAry(int ** Ary) {
    for (int i = 0; i < power2Size; i++) {
      for (int j = 0; j < power2Size; j++) {
        Ary[i][j] = 0;
      }
    }
  }

};

int main(int argc, char * argv[]) {
  ifstream inFile;
  ofstream outFile1, outFile2, outFile3;

  inFile.open(argv[1]);
  outFile1.open(argv[2]);
  outFile2.open(argv[3]);
  outFile3.open(argv[4]);

  int numRows, numCols, minVal, maxVal;
  inFile >> numRows;
  inFile >> numCols;
  inFile >> minVal;
  inFile >> maxVal;

  QuadTree * qt = new QuadTree(numRows, numCols, minVal, maxVal);

  outFile2 << "power2Size: " << qt -> power2Size << endl; //power2Size done in constructor
  qt -> zero2DAry(qt -> imgAry);
  qt -> zero2DAry(qt -> newimgAry);
  qt -> loadImage(inFile, qt -> imgAry);

  qt -> QtRoot = qt -> buildQuadTree(qt -> imgAry, 0, 0, qt -> power2Size, outFile2);

  outFile1 << "preOrder:" << endl;
  qt -> preOrder(qt -> QtRoot, outFile1);
  outFile1 << endl;

  outFile1 << "postOrder:" << endl;
  qt -> postOrder(qt -> QtRoot, outFile1);
  outFile1 << endl;
  qt -> getLeaf(qt -> QtRoot, qt -> newimgAry);

  outFile3 << "imgARY: " << endl; //PRINTS IMGARY
  for (int i = 0; i < qt -> power2Size; i++) { //row
    for (int j = 0; j < qt -> power2Size; j++) { //column
      outFile3 << qt -> imgAry[i][j] << " "; //
      if (j == qt -> power2Size - 1) {
        outFile3 << endl;
      }
    }
  }
  outFile3<<endl;
  outFile3 << "newimgARY: " << endl; //PRINTS NEWIMGARY
  for (int i = 0; i < qt -> power2Size; i++) { //row
    for (int j = 0; j < qt -> power2Size; j++) { //column
      outFile3 << qt -> newimgAry[i][j] << " "; //
      if (j == qt -> power2Size - 1) {
        outFile3 << endl;
      }
    }
  }

  inFile.close();
  outFile1.close();
  outFile2.close();
  outFile3.close();

  return 0;
};
