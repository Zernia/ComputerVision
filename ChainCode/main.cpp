
#include <iostream>
#include <fstream>
using namespace std;
int numRows;
int numCols;
int maxVal;
int minVal;
int ** imageAry;
int counter;
int zeroTable[8] = {6,0,0,2,2,4,4,6};//index is the dir->currentP to lastzero



class Image{
public:

    //int **imageAry;
    int **CCAry;
    
    
    Image(ifstream &inFile){
        inFile>>numRows>>numCols>>minVal>>maxVal;
        imageAry = new int*[numRows+2];
        CCAry = new int*[numRows+2];
        for(int i =0; i <numRows+2;i++){
            imageAry[i] =new int[numCols+2]();
            CCAry[i] = new int[numCols+2]();
        }
    }
    
    void loadImage(ifstream &inFile, int ** Ary){
        for(int i =1; i <numRows+1; i++){
            for(int j =1; j <numCols+1;j++){
                inFile>>Ary[i][j];
            }
        }
        
    }
};

class ConnectCC{
public:
    int label;
    int CCnumber;
    int numbPixels;
    int boundingBox_minRow;
    int boundingBox_minCol;
    int boundingBox_maxRaw;
    int boundingBox_maxCol;
    int labelRow;
    int labelCol;
    int labelMin;
    int labelMax;
    
    ConnectCC(ifstream &inFile){
        inFile>>labelRow>>labelCol>>labelMin>>labelMax>>CCnumber;
        
    }
    
    void clearCC(int **Ary){
        for(int i =0; i <numRows+2;i++){
            for(int j =0; j <numCols+2;j++){
                Ary[i][j]= 0;
            }
        }
    }
    
    void loadCC(int **imgAry, int **cAry, int minR, int minC, int maxR, int maxC){

        for(int i =minR+1; i <maxR+2; i++){
            for(int j =minC+1; j <maxC+2;j++){

                cAry[i][j] =imgAry[i][j];
                
            }

        }
        
    }
};

class ChainCode{
public:
    class Point{
    public:
    int row, col,value;
    Point(){
        row = col = 0;
        value =0;
        
    }
    Point(int r, int c){
        row = r;
        col = c;
        //value = imageAry[row][col];
        
    }
        Point& operator = (const Point& p){
            this->row = p.row;
            this->col = p.col;
            this->value = p.value;
            return *this;
        }
        bool operator ==(const Point &p){
            return row==p.row&&col==p.col&&value==p.value;
        }
    };
//    Point neightBorCoord[8] = {Point(0,1), Point(-1,1), Point(-1,0),
//                                Point(-1, -1), Point(0, -1), Point(1, -1),
//                                Point(1, 0), Point(+1, 1)};
    Point neighBorCoord[8];
    Point startP;
    Point currentP;//current non zero pixel
    Point nextP;//next non zero pixel
    int lastQ;//0--7//lastzero to currentP
    int nextQ;//I add it
   // int zeroTable[8] = {6,0,0,2,2,4,4,6};//index is the dir->currentP to lastzero//non static
    //value is dir->nextP to lastzero

    
    
    int nextDir;//dir->currentp's neighbor
    int PchainDir;//dir for current to next
    ChainCode(){
        lastQ = 0;
        PchainDir = 0;
        counter =0;
    }
    void getChainCode(int ccLabel, int **cAry, int minR, int minC, int maxR, int maxC,ofstream &debugFile, ofstream& chainCodeFile){
        int label = ccLabel;
        bool broke = false;
        for(int i = minR+1;i<=maxR+1;i++){
            for(int j =minC+1;j<=maxC+1;j++){
                if(cAry[i][j]==label){
                    counter =0;
                    startP.row = i;
                    startP.col=j;
                    currentP.row=i;
                    currentP.col=j;
                    lastQ =4;
                   // cout<<label<<" "<<i<<" "<<j<<" ";
                    chainCodeFile<<label<<" "<<i<<" "<<j<<" ";
                    debugFile<<label<<" "<<i<<" "<<j<<endl;
                    
                    broke=true;
                    break;
                    
                }
            }

            if(broke)
                break;
                
        }
        do{
        nextQ=(lastQ+1)%8;
            PchainDir = findNextP(currentP,nextQ,nextP,cAry,ccLabel);
            counter++;
            //cout<<PchainDir;
            chainCodeFile<<PchainDir;
            debugFile<<PchainDir<<" ";
            if((counter)%20==0){
                //cout<<endl;
                debugFile<<endl;
            }

            currentP.value=-currentP.value;
            //lastQ = zeroTable[(PchainDir+7)%8];
            if(PchainDir==0)
                lastQ=zeroTable[7];
            else
                lastQ = zeroTable[PchainDir-1];
            currentP=nextP;
    
        }while((currentP.row!=startP.row)||(currentP.col!=startP.col));
        
        
    }

    
    int findNextP(Point &currentP, int nextQ, Point &nextP, int **cAry,int ccLabel){
        loadNeighborCoord(currentP,cAry);
        int i =0;
        
        while(i<8){
            if(neighBorCoord[(nextQ+i)%8].value==ccLabel)
                break;
            
            else{
            i++;
            }
        }
        
        nextP=neighBorCoord[(nextQ+i)%8];
        
        return (nextQ+i)%8;
        
    }
    void loadNeighborCoord(Point &currentP,int **cAry){
        neighBorCoord[0] = *new Point(currentP.row,currentP.col+1);
        neighBorCoord[0].value =cAry[neighBorCoord[0].row][neighBorCoord[0].col];
        neighBorCoord[1] = *new Point(currentP.row-1,currentP.col+1);
        neighBorCoord[1].value =cAry[neighBorCoord[1].row][neighBorCoord[1].col];
        neighBorCoord[2] = *new Point(currentP.row-1,currentP.col);
        neighBorCoord[2].value =cAry[neighBorCoord[2].row][neighBorCoord[2].col];
        neighBorCoord[3] = *new Point(currentP.row-1,currentP.col-1);
        neighBorCoord[3].value =cAry[neighBorCoord[3].row][neighBorCoord[3].col];

        neighBorCoord[4] = *new Point(currentP.row,currentP.col-1);
        neighBorCoord[4].value =cAry[neighBorCoord[4].row][neighBorCoord[4].col];

        neighBorCoord[5] = *new Point(currentP.row+1,currentP.col-1);
        neighBorCoord[5].value =cAry[neighBorCoord[5].row][neighBorCoord[5].col];

        neighBorCoord[6] = *new Point(currentP.row+1,currentP.col);
        neighBorCoord[6].value =cAry[neighBorCoord[6].row][neighBorCoord[6].col];

        neighBorCoord[7] = *new Point(currentP.row+1,currentP.col+1);
        neighBorCoord[7].value =cAry[neighBorCoord[7].row][neighBorCoord[7].col];
    }
    
   
        
      
    
   
    
};

int main(int args, char**argv){
    ifstream labelFile, propFile;
    ofstream chainCodeFile, deBugFile;
    
    labelFile.open(argv[1]);
    propFile.open(argv[2]);
    chainCodeFile.open(argv[3]);
    
//    labelFile.open("chainCodeImg2CC.txt");
//    propFile.open("chainCodeImg2Property.txt");
//    chainCodeFile.open("chainCode.txt");
////

    
    string chainCodeFileName = string(argv[3]).substr(0,string(argv[3]).size()-4);
   // string chainCodeFileName = string("chainCode.txt").substr(0,string("chaiCcode.txt").size()-4);

    
    string deBugFileName = chainCodeFileName+"_debugFile.txt";
    
    deBugFile.open(deBugFileName);
    
    Image *img =new Image(labelFile);
    ConnectCC *ccc = new ConnectCC(propFile);
    ChainCode *chainCode = new ChainCode();
    
    chainCodeFile<<numRows<<" "<<numCols<<" "<<minVal<<" "<<maxVal<<endl;
    deBugFile<<numRows<<" "<<numCols<<" "<<minVal<<" "<<maxVal<<endl;
   // cout<<numRows<<" "<<numCols<<" "<<minVal<<" "<<maxVal<<endl;
    
    img->loadImage(labelFile, imageAry);
    

    

    
    int CC=1;
    while(CC<=ccc->CCnumber){
        propFile>>ccc->label>>ccc->numbPixels>>ccc->boundingBox_minRow>>ccc->boundingBox_minCol>>ccc->boundingBox_maxRaw>>ccc->boundingBox_maxCol;
        
        ccc->clearCC(img->CCAry);
        ccc->loadCC(imageAry, img->CCAry, ccc->boundingBox_minRow, ccc->boundingBox_minCol, ccc->boundingBox_maxRaw, ccc->boundingBox_maxCol);
        

        chainCode->getChainCode(ccc->label, img->CCAry,ccc->boundingBox_minRow,ccc->boundingBox_minCol,ccc->boundingBox_maxRaw,ccc->boundingBox_maxCol,deBugFile,chainCodeFile);
        //cout<<endl;
        chainCodeFile<<endl;
        deBugFile<<endl<<endl;
        
        
        CC++;
    }
    deBugFile.close();
    chainCodeFile.close();
    
    
    
    
    
}
