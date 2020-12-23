//
//  main.cpp
//  Project11
//
//  Created by zerniaovo on 4/27/20.
//  Copyright © 2020 zerniaovo. All rights reserved.
//

#include <iostream>
#include <fstream>
using namespace std;

int **imgAry;

int *HPP;
int *VPP;
int *HPPbin;
int *VPPbin;
int *HPPMorph;
int *VPPMorph;
int *tempAry;



int HPPruns;
int VPPruns;
int numRows;
int numCols;
int minVal;
int maxVal;
int thresholdValue;
int Origin = 1;//1,1,1 the middle
int structEle [3];
int structLengh = 3;

int readingDir;




class ImagePP{
public:
    class Box{
    public:
        int minR;
        int minC;
        int maxR;
        int maxC;
        Box(){
            minR=minC=maxR=maxC=0;
        }

    };
    class BoxNode{
    public:
        int boxType;
        Box *BBbox;
        BoxNode *next;
        BoxNode(Box *Boxn){
            boxType = 3;
            BBbox = Boxn;
            next = NULL;
        }
        BoxNode(){
            boxType =3;
            BBbox = NULL;
            next = NULL;
        }
        
    };
    class BoxQ{
    public:
        BoxNode* Qfront;
        BoxNode* Qback;
        
        BoxQ(BoxNode *qfront){
            Qfront = qfront;
            Qback = Qfront;
        }
        
        
        void insert(BoxNode *newBoxNode){
            Qback->next = newBoxNode;
            Qback = newBoxNode;
        }
        
    };
    Box *BoundBox;
    BoxQ *BoxQueue;
    
    ImagePP(ifstream &inFile, int thresholdvalue){
        inFile>>numRows>>numCols>>minVal>>maxVal;
        imgAry = new int *[numRows+2];
        for(int i =0; i <numRows+2; i++){
            imgAry[i] = new int [numCols+2]();
        }
        
        HPPruns = VPPruns = 0;
        
        HPP = new int [numRows+2]();
        VPP = new int [numCols+2]();
        
        HPPbin = new int [numRows+2]();
        VPPbin = new int [numCols+2]();
        
        HPPMorph = new int [numRows+2]();
        VPPMorph = new int [numCols+2]();
        
        BoxQueue = new BoxQ(new BoxNode());
        
        
        
        
    }
    
    void loadImage(ifstream &inFile, int **Ary){
        for(int i = 1; i<numRows+1; i ++){
            for(int j =1; j <numCols+1;j++){
                inFile>>Ary[i][j];
            }
        }
        
    }
    
    void computeVPP(int **Ary, int *vAry){
        for(int i =0; i <numRows+2;i++){
            for(int j = 0; j <numCols+2;j++){
                if(Ary[i][j]==1){
                    vAry[j]++;
                }
            }
        }
    }
    
    void computeHPP(int **Ary, int *hAry){
        for(int i =0; i <numRows+2;i++){
            for(int j = 0; j <numCols+2;j++){
                if(Ary[i][j]==1){
                    hAry[i]++;
                }
            }
        }
    }
    
    void printPP(ofstream &outFile, int *Ary, int number){
        //outFile<<numRows<<" "<<numRows<<" "<<minVal<<" "<<maxVal<<endl;
        //cout<<numRows<<" "<<numRows<<" "<<minVal<<" "<<maxVal<<endl;

//        for(int i =1; i <number+1;i++){
//
//            outFile<<i-1<<" "<<Ary[i]<<endl;
//            cout<<i-1<<" "<<Ary[i]<<endl;
//        }

        for(int i =1; i <number+1;++i)
            if(i-1<10)
                outFile<< i-1<<" |";
                //cout<<i-1<<"  ";
            
            else
                outFile<<i-1<<"|";
                //cout<<i-1<<" ";
            

        for(int i =0; i<number+1;++i)
            outFile<<" ";
            //cout<<" ";
        for(int i =1; i <number+1;++i){
            if(Ary[i]<10)
                outFile<<Ary[i]<<" |";
                //cout<<Ary[i]<<"  ";
            
            else
                outFile<<Ary[i]<<"|";
               // cout<<Ary[i]<<" ";
        }
            //cout<<"\n\n";
           outFile<<"\n\n";
            
        
    }
    
    void changeThreshold(int *Ary, int *Arybin, int number){
        for(int i =1; i <number+1;i++){
            if(Ary[i]>=thresholdValue){
                //cout<<"the number is "<<Ary[i]<<endl;
                Arybin[i] =1;
            }
            else{
                Arybin[i] = 0;
               // cout<<"the num is "<<Ary[i]<<endl;
            }
        }
        
    }
    //the stucture element is 111 origin is 1
    void MorphologyClosing(int number, int *Ary, int *mAry){
        int *tempAry = new int [number+structLengh]();
        for(int i = Origin; i <number+Origin;i++){
            if(Ary[i]>=1){
        computeDilation(i, number, Ary, tempAry);
        computeErosion(i, number, tempAry, mAry);
            }
        }
        
    }
    
    void computeDilation(int m, int number, int *Ary, int *mAry){
        for(int i = m; i<number+Origin;i++){
            if(Ary[i]>=1)
                dilation(Ary, mAry,i);
        }
    }
    
    void dilation(int *Ary, int *mAry, int n){
        int numOffset =0;
        numOffset = n+(0-Origin);
        for(int r =0; r<structLengh;r++){
            if(structEle[r]>=1)
                mAry[numOffset+r] = 1;
        }
        
        
    }
    void computeErosion(int m, int number, int *Ary, int *mAry){
        for(int i = m; i<number+Origin;i++){
            if(Ary[i]>=1)
                erosion(Ary,mAry,i);
        }
    }
    
    void erosion(int *Ary, int *mAry, int n){
        bool good  = true;
        int numberOffset = 0;
        numberOffset = n+(0-Origin);
        for(int r = 0; r<structLengh;r++){
            if(structEle[r]>=1&&Ary[numberOffset+r]<=0)
                good = false;
        }
        if(good){
            mAry[n]=1;
        }
    }
    
    int computeRuns(int *Ary, int number){
        int counter =0;

        for(int i =1; i<number+1;i++){
            if(Ary[i+1]==1&&Ary[i]==0){
                counter++;
            }
        }
        return counter;
    }
    
    void determineTheReadingDirection(ofstream &outFile){
        int factor =3;
        HPPruns = computeRuns(HPPMorph,numRows);
        VPPruns = computeRuns(VPPMorph, numCols);
        
        if(HPPruns>=factor*VPPruns){
            readingDir=1;//horizontal
            outFile<<"The Reading direction is Horizontal"<<endl;
            //cout<<"The Reading direction is Horizontal"<<endl;
            outFile<<endl;

        }
            else if(VPPruns>=factor*HPPruns){
                readingDir=2;//vertical
                outFile<<"The Reading direction is Vertical"<<endl;
                //cout<<"The Reading direction is Vertical"<<endl;
                outFile<<endl;

            }
            else{
                readingDir=3;
                outFile<<"Can not determine the reading direction and exit the Program";
                //cout<<"Can not determine the reading direction and exit the Program";
                outFile<<endl;
                exit(1);

            }
                    
    }
    
    
    
    
    
    void printBoxInfo(ofstream& outFile2){
        BoxNode* newBNode = BoxQueue->Qfront->next;
        while (newBNode) {
            //cout<<newBNode->BBbox->minR<<endl;
            //cout<<newBNode->BBbox->maxR<<endl;
            //cout<<newBNode->BBbox->minC<<endl;
//            cout<<newBNode->boxType<<endl;
//            cout<<newBNode->BBbox->minR<<" "<<newBNode->BBbox->minC<<" ";
//            cout<<newBNode->BBbox->maxR<<" "<<newBNode->BBbox->maxC<<endl;
            
            outFile2<<newBNode->boxType<<endl;
            outFile2<<newBNode->BBbox->minR<<" "<<newBNode->BBbox->minC<<" ";
            outFile2<<newBNode->BBbox->maxR<<" "<<newBNode->BBbox->maxC<<endl;

            

            newBNode = newBNode->next;

        }

        
    }
    
     void drawBoxes(ofstream &outFile1){
         
             BoxNode* newBNode = BoxQueue->Qfront->next;
             while (newBNode) {

                 for(int i =(newBNode->BBbox->minR);i<(newBNode->BBbox->maxR)+1;i++){
                     imgAry[i+1][newBNode->BBbox->minC+1]=1;
                     imgAry[i+1][newBNode->BBbox->maxC+1]=1;
                 }
                 for(int j =(newBNode->BBbox->minC);j<(newBNode->BBbox->maxC)+1;j++){
                     imgAry[newBNode->BBbox->minR+1][j+1]=1;
                     imgAry[newBNode->BBbox->maxR+1][j+1]=1;
                 }
                 newBNode = newBNode->next;

             }
             for(int i =1; i <numRows+1; i++){
                 for(int j =1; j <numCols+1; j++){
                     if(imgAry[i][j]==1){
                    // cout<<imgAry[i][j]<<" ";
                     outFile1<<imgAry[i][j]<<" ";
                     }
                     else{
                         //cout<<"  ";
                         outFile1<<"  ";
                     }
                 }
                 //cout<<endl;
                 outFile1<<endl;
             }
             
         }
    void boundBoxLine(){
              Box *box;
              Box *originalBox = new Box();
              BoxNode *boxnode;
              
        for(int j=1;j<numCols+1; j++){
           // cout<<"j is "<<j<<endl;
        
          if(VPPMorph[j]==1){
            originalBox->maxC = originalBox->maxC>j?originalBox->maxC:j-1;

                }

            }
        for(int j =1; j<numCols+1;j++){
             if(VPPMorph[j]==1){
                originalBox->minC = j-1;
                 break;

                }
            }
                  
        for(int i =1; i<numRows+1;i++){
            if(HPPMorph[i]==1){
                originalBox->maxR = originalBox->maxR>i?originalBox->maxR:i-1;
            }
        }
        for(int i =1; i<numRows+1;i++){
            if(HPPMorph[i]==1){
                originalBox->minR = i-1;
                break;
            }
            
        }
        if(readingDir==1){
            for(int i =1; i<numRows+1;i++){
                if((HPPMorph[i]==1)&&(HPPMorph[i-1]==0)){
                    box = new Box();
                    boxnode = new BoxNode(box);
                    //cout<<i-1<<endl;
                    box->minR=i-1;
                    for(int r = box->minR; r<numRows;r++){
                        if((HPPMorph[r+1]==0)&&(HPPMorph[r]==1)){
                            box->maxR=r-1;
                            //cout<<r-1<<endl;
                            box->minC=originalBox->minC;
                            box->maxC=originalBox->maxC;
                            break;
                        }
                    }
                    BoxQueue->insert(boxnode);

                    
                }

            }
            
            
        }
        
        if(readingDir==2){
            for(int j =1; j<numCols+1;j++){
                if((VPPMorph[j]==1)&&(VPPMorph[j-1]==0)){
                    box = new Box();
                    boxnode = new BoxNode(box);
                    //cout<<j-1<<endl;
                    box->minC=j-1;
                    for(int c = box->minC; c<numCols;c++){
                        if((VPPMorph[c+1]==0)&&(VPPMorph[c]==1)){
                            box->maxC=c-1;
                           // cout<<c-1<<endl;
                            box->minR=originalBox->minR;
                            box->maxR=originalBox->maxR;
                            break;
                        }
                    }
                    BoxQueue->insert(boxnode);

                    
                }

            }
            
            
        }
        
        
                 // cout<< "the min C is"<<originalBox->minC<<endl ;

                  //cout<< "the max C is"<<originalBox->maxC<<endl ;
                  
                  
                
        
    
    }
         
        
    

    
    

    
};

int main(int argc, char * argv[]) {
    ifstream inFile;
    ofstream outFile1, outFile2;

    
//    inFile.open("textLineDetection_data2.txt");
//    thresholdValue = 3;
//    outFile1.open("textLineDetection_data2OutFile1.txt");
//    outFile2.open("textLineDetection_data2OutFile2.txt");

    
    inFile.open(argv[1]);
    thresholdValue = atoi(argv[2]);
    outFile1.open(argv[3]);
    outFile2.open(argv[4]);
    structEle[0] = 1;
    structEle[1] = 1;
    structEle[2] = 1;
    
    Origin = 1;
    ImagePP * imgpp = new ImagePP(inFile,thresholdValue);
    imgpp->loadImage(inFile, imgAry);

    imgpp->computeHPP(imgAry, HPP);
    imgpp->computeVPP(imgAry, VPP);
    
    outFile2<<"HPP Array is:"<<endl;
    imgpp->printPP(outFile2, HPP, numRows);
    outFile2<<endl;
    outFile2<<"VPP Array is:"<<endl;
    imgpp->printPP(outFile2, VPP, numCols);
    outFile2<<endl;
    
    imgpp->changeThreshold(HPP, HPPbin, numRows);
    imgpp->changeThreshold(VPP, VPPbin, numCols);
    outFile2<<"HPPBin Array is:"<<endl;
    imgpp->printPP(outFile2, HPPbin, numRows);
    outFile2<<endl;
    outFile2<<"VPPBin Array is:"<<endl;
    imgpp->printPP(outFile2, VPPbin, numCols);
    outFile2<<endl;
    
    imgpp->MorphologyClosing(numRows, HPPbin, HPPMorph);
    //imgpp->printPP(outFile2, HPPMorph, numRows);
    outFile2<<endl;
    imgpp->MorphologyClosing(numCols, VPPbin, VPPMorph);
    //imgpp->printPP(outFile2, VPPMorph, numCols);
    outFile2<<endl;
    imgpp->determineTheReadingDirection(outFile2);

    
    imgpp->boundBoxLine();
    
    outFile2<<"The box information is: "<<endl;
    imgpp->printBoxInfo(outFile2);
    imgpp->drawBoxes(outFile1);
//    for(int i =0; i <numRows+2;i++){
//        for(int j =0; j <numCols+2;j++){
//            cout<<imgAry[i][j];
//        }
//        cout<<endl;
//    }
    
    
    
    
    outFile1.close();
    outFile2.close();
    inFile.close();
    
    
}
