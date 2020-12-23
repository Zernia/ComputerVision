
#include <iostream>
#include <fstream>

using namespace std;

class CCLabel{
    public:
    int numRows, numCols, minVal, maxVal, newMin, newMax;
    int newLabel;
    int trueNumCC;
    int ** zeroFramedAry;
    int numNb;
    int minLabel;
    int nonZeroCount;
    
    int * NonZeroNeighborAry;
    
    int * EQAry;
    int connectness;
    
    int ** CCproperty;
    int * Property;
    int realLabel;

    int minRow;
    int minCol;
    int maxRow;
    int maxCol;
    
    int numPixels;
    
    CCLabel(ifstream &inFile, int Connectness){
        numRows=numCols= minVal= maxVal= newMin= newMax=0;
        numNb = 5;
        
        newLabel=0;
        connectness = Connectness;
       // cout<<connectness;
       inFile>>numRows>>numCols>>minVal>>maxVal;
        
        NonZeroNeighborAry = new int[numNb + 1]();
        
       EQAry = new int[numRows*numCols]();
       for(int i = 1; i <numRows*numCols;i++){
            EQAry[i] = 1;
        }
        
       zeroFramedAry = new int*[numRows+2];
       for(int i = 0; i <numRows+2;i++){
            zeroFramedAry[i] = new int[numCols+2];
        }
        

        Property = new int[6];//numpixel,ulr,ulc,lrr,lrc,


    }
    ~CCLabel(){
        delete[] NonZeroNeighborAry;
        delete[] EQAry;
        delete[] CCproperty;
        for (int i = 0; i < numRows + 2; ++i)
            delete[] zeroFramedAry[i];
    }
    void zero2D(int ** ary){
        for(int i = 0; i < numRows+2; i++){
            for(int j = 0; j< numCols+2; j++){
                ary[i][j] = 0;

            }
        }
    }
    
    void minus1D(int * ary){
        for(int i = 0; i <numNb+1; i ++){
            ary[i] = -1;
            //cout<<ary[i];
        }
    }
    
    void loadImage(ifstream & inFile, int ** zeroFramedAry){
        for(int i = 1; i < numRows+1; i++){
            for(int j = 1 ; j < numCols+1; j++){
                inFile>>zeroFramedAry[i][j];
                //cout<<zeroFramedAry[i][j]<<" ";
            }
           // cout<<endl;
        }
    }
    
    int connectPass1(int Conectness, int ** Ary, int * nonZeroAry){
    
        newLabel = 0;
        bool diffFlag=true;
        for(int i = 1; i <numRows+1; i++){
            for(int j = 1; j <numCols+1; j++){
                if( Ary[i][j]>0){
                    
                    minLabel = loadNonZeroPass1(Ary, Conectness,i,j,nonZeroAry,diffFlag);
                    //cout<<minLabel<<" minLabel in connectPass1"<<endl;
                   // cout<<nonZeroCount<<"nonZeroCount in pass1"<<endl;
                    if(minLabel ==0){
                        newLabel++;
                        Ary[i][j] = newLabel;
                    }
                    else if(minLabel>0){
                        
    
                        Ary[i][j] = minLabel;
                        
                        if(diffFlag ==true){
                            
                            updateEQ(EQAry, nonZeroAry,nonZeroCount);
                            
                        }
                        
                    }
                }
                
            }
        }
        return newLabel;
        
    }
    
    int loadNonZeroPass1(int ** Ary, int Connectness, int i, int j, int * nonZeroAry, bool diffFlag){
        minus1D(nonZeroAry);
        nonZeroCount = 0;
        diffFlag = false;
        
        if(Ary[i-1][j]>0){
            //cout<<nonZeroAry[nonZeroCount]<<"no 0ary in pass"<<endl;
            //cout<<Ary[i][j]<<"ary in pass"<<endl;
            nonZeroAry[nonZeroCount] = Ary[i-1][j];
            nonZeroCount++;
        }
        
        if(Ary[i][j-1]>0){
            nonZeroAry[nonZeroCount] = Ary[i][j-1];
            nonZeroCount++;
        }
        
        if(Connectness ==8){
            if(Ary[i-1][j-1]>0){
                nonZeroAry[nonZeroCount] = Ary[i-1][j-1];
                nonZeroCount++;
            }
            if(Ary[i-1][j+1]>0){
                nonZeroAry[nonZeroCount] = Ary[i-1][j+1];
                nonZeroCount++;
            }
        }
        if(nonZeroCount<=0){
            return 0;
        }
        minLabel = nonZeroAry[0];
        diffFlag = false;
        int index = 1;
        //cout<<nonZeroCount<<"NonzeroCount in Load";
        while(index<nonZeroCount){
                    //cout<<nonZeroCount<<"NonzeroCount in Load";
          //  cout<<minLabel<<" minLabel in load";
            //cout<<nonZeroAry[index]<<"nonZeroArray in load"<<endl;
            if(minLabel!=nonZeroAry[index]){
                diffFlag = true;
            }
            if(minLabel > nonZeroAry[index]){
                minLabel = nonZeroAry[index];
               // cout<<minLabel;
            }
            index++;
        }
       // cout<<minLabel<<" minLabei in Load "<<endl;
        return minLabel;
    }
    
    void updateEQ(int * EQAry, int * nonZeroAry, int nonZeroCount){
        int index = 0;
       // cout<<minLabel<<"minLabel in Update "<<endl;
    
       // cout<<nonZeroCount<<"nonZeroCount in update"<<endl;
        while((index<nonZeroCount)&&(nonZeroAry[index]!=-1)){
           // cout<<minLabel;
            EQAry[nonZeroAry[index]] = minLabel;
            index++;
        }
    }
    
    void prettyPrint(ofstream & outputFile){
        for(int i = 1 ;i<numRows+1;i++){
            for(int j = 1; j <numCols+1;j++){
                if(zeroFramedAry[i][j]>0){
                    //cout<<zeroFramedAry[i][j]<<" ";
                    outputFile<<zeroFramedAry[i][j]<<" ";

                    
                }
                else{
                    //cout<<"  ";
                    outputFile<<"  ";

                }
            }
            //cout<<endl;
            outputFile<<endl;

        }
    }
    
    void printEQAry( ofstream & outFile) {
        outFile << "\n     ";
        //cout << "\n     ";

        for (int i = 0; i <= newLabel; ++i)
            if (i < 10){
                outFile << i << "  ";
                //cout<<"";
                //cout << i << "  ";
            }
            else
                outFile << i << " ";
               //cout << i << " ";

        outFile << "\n     ";
        //cout << "\n     ";

        for (int i = 0; i <= newLabel; ++i)
            outFile << "";
           //cout << "";

        outFile << "\nEQAry|";
        //cout << "\nEQAry|";
        for (int i = 0; i <= newLabel; ++i) {
            if (EQAry[i] < 10)
                outFile << EQAry[i] << "  ";
                //cout << EQAry[i] << "  ";

            else
                outFile << EQAry[i] << " ";
                //cout << EQAry[i] << " ";

        }
        outFile << "\n\n";
        //cout << "\n\n";

    }
    
    int connectPass2(int Connectness, int ** Ary, int * nonZeroAry){
        bool diffFlag = true;
        for(int i = numRows; i >=1; i--){
            for(int j = numCols; j >=1; j--){
                if(Ary[i][j]>0){
                    minLabel = loadNonZeroPass2(Ary, Connectness, i, j, nonZeroAry, diffFlag);
                    if(minLabel !=Ary[i][j]){
                        Ary[i][j] = minLabel;
//                        if(diffFlag ==true){
//                            updateEQ(EQAry, nonZeroAry, nonZeroCount);
//                        }
                    }
                                            if(diffFlag ==true){
                        updateEQ(EQAry, nonZeroAry, nonZeroCount);
                    }                }
            }
        }
        return newLabel;
    }
    
    int loadNonZeroPass2(int ** Ary, int Connectness, int i, int j,int * nonZeroAry, bool diffFlag){
        minus1D(nonZeroAry);
        nonZeroCount = 0;
        diffFlag = false;
        
        nonZeroAry[nonZeroCount] = Ary[i][j];
        nonZeroCount++;
        
        if(Ary[i+1][j]>0){
            nonZeroAry[nonZeroCount] = Ary[i+1][j];
            nonZeroCount++;
        }
        if(Ary[i][j+1]>0){
            nonZeroAry[nonZeroCount] = Ary[i][j+1];
            nonZeroCount++;
        }
        if(Connectness ==8){
            if(Ary[i+1][j-1]>0){
                nonZeroAry[nonZeroCount] = Ary[i+1][j-1];
                nonZeroCount++;
            }
            if(Ary[i+1][j+1]>0){
                nonZeroAry[nonZeroCount] = Ary[i+1][j+1];
                nonZeroCount++;
            }
        }
        minLabel = nonZeroAry[0];
        diffFlag = false;
        
        int index = 1;
        while(index<nonZeroCount){
//            if(minLabel>nonZeroAry[index]){
//                minLabel = nonZeroAry[index];
//                diffFlag=true;
//            }
            if(minLabel!=nonZeroAry[index]){
                diffFlag = true;
            }
            if(minLabel > nonZeroAry[index]){
                minLabel = nonZeroAry[index];
            }
            index++;
        }
        return minLabel;
    }
    
    int ManageEQAry(int * EQAry, int newLabel){
        trueNumCC = 0;
        newMax = 0;
        int index = 1;
        while(index<=newLabel){
            if(index!= EQAry[index]){
            EQAry[index] = EQAry[EQAry[index]];
        }
        else{
                trueNumCC++;
            newMax++;
                EQAry[index] = trueNumCC;
            }
            index++;

        }
        return trueNumCC;
    
    }
    void connectPass3(int ** Ary){
        CCproperty = new int* [trueNumCC+1]();
        for(int i = 0; i <=trueNumCC;i++){
            CCproperty[i] = new int[6]();
           CCproperty[i][2] = numRows;
            CCproperty[i][3] = numCols;
        }
    
                for(int i = 1; i <numRows+1;i++){
                for(int j = 1; j <numCols+1;j++){
                    Ary[i][j] = EQAry[Ary[i][j]];
                    //cout<<Ary[i][j];
                      if(Ary[i][j]!=0){
                          //Property[zeroFramedAry[i][j]]++;
                          
                            if(zeroFramedAry[i][j]>=0){
                               // Property[0] = EQAry[Ary[i][j]];//label
                               // CCproperty[zeroFramedAry[i][j]][0] = EQAry[Ary[i][j]];
                                Property[1]++;//numP
                                CCproperty[zeroFramedAry[i][j]][1]++;
                            if(CCproperty[zeroFramedAry[i][j]][2]>i-1){
                                Property[2] = i-1;
                                CCproperty[zeroFramedAry[i][j]][2] =i;
                                
                                //cout<<Property[2]<<endl;
                            }//ulr
                            if(CCproperty[zeroFramedAry[i][j]][3]>j-1){
                                Property[3] = j-1;
                                CCproperty[zeroFramedAry[i][j]][3] = j;
                                //cout<<Property[3]<<endl;

                            }
                            if(CCproperty[zeroFramedAry[i][j]][4]<i-1){
                                Property[4] = i-1;
                                CCproperty[zeroFramedAry[i][j]][4] = i;
                            }
                            if(CCproperty[zeroFramedAry[i][j]][5]<j-1){
                                Property[5] = j-1;
                                CCproperty[zeroFramedAry[i][j]][5]= j;
                            }
                        }
                    
                }
            }
                //cout<<endl;
            
        }

    }
    
        void drawBoxes(int **Ary, int **CC) {
//            for(int i = 0;i<numRows;i++){
//                for(int j = 0; j <numCols;j++){
//                    cout<<Ary[i][j]<<" ";
//                }
//                cout<<endl;
//            }
            int index = 1;
            while(index<=trueNumCC){
                minRow = CCproperty[index][2];//not add 1
                minCol = CCproperty[index][3];//not add 1
                maxRow = CCproperty[index][4]+1;
                maxCol = CCproperty[index][5]+1;//just add max row and max Col the result will be better;
                for(int j = minCol;j<maxCol;j++){
                    Ary[minRow][j] = index;
                    Ary[maxRow][j] = index;
                }
                for(int i = minRow;i<=maxRow;i++){
                    Ary[i][minCol] = index;
                    Ary[i][maxCol] = index;
                }
                index++;
            }
        }
    
    
    void printImage(ofstream &outFile){
        //cout<<numRows<<" "<<numCols<<" "<<newMin<<" "<<newMax<<" "<<endl;
        outFile<<numRows<<" "<<numCols<<" "<<newMin<<" "<<newMax<<" "<<endl;

        for(int i = 1; i <=numRows; i++){
            for(int j = 1; j <= numCols;j++){
                //cout<<zeroFramedAry[i][j]<<" ";
               outFile<<zeroFramedAry[i][j]<<" ";

            }
            //cout<<endl;
            outFile<<endl;

        }
        
    }
    
    void printCCproperty(ofstream & outFile){
        //cout<<numRows<<" "<<numCols<<" "<<newMin<<" "<<newMax<<endl;
        outFile<<numRows<<" "<<numCols<<" "<<newMin<<" "<<newMax<<endl;

        for(int i =0; i <=newMax;i++){
//            cout<<i <<endl;
//            cout<<CCproperty[i][0]<<endl;
//            cout<<CCproperty[i][1]<<endl;
//            cout<<CCproperty[i][2]<<" "<<CCproperty[i][3]<<endl;
//            cout<<CCproperty[i][4]<<" "<<CCproperty[i][5]<<endl;
            outFile<<i <<endl;
            //cout<<CCproperty[i][0]<<endl;
            outFile<<CCproperty[i][1]<<endl;
            outFile<<CCproperty[i][2]<<" "<<CCproperty[i][3]<<endl;
            outFile<<CCproperty[i][4]<<" "<<CCproperty[i][5]<<endl;
        
            }
        }
        
     
    

};

int main(int argc, char** argv){
    ifstream inFile;
    ofstream prettyPrintFile, labelFile, propertyFile;
    
    
    //inFile.open("/Users/Niya/Desktop/opencvc++/ConnectedComponent_Data.txt");
    inFile.open(argv[1]);
    int Connectness = atoi(argv[2]);
   // int Connectness = 8;

    prettyPrintFile.open(argv[3]);
    labelFile.open(argv[4]);
    propertyFile.open(argv[5]);

    CCLabel * cclabel = new CCLabel(inFile, Connectness);
    //cout<<cclabel.numRows;
    
    cclabel->loadImage(inFile,cclabel->zeroFramedAry);
    prettyPrintFile<<endl;
    prettyPrintFile<<"ConnectPass1";
    cclabel->connectPass1(Connectness, cclabel->zeroFramedAry, cclabel->NonZeroNeighborAry);
    
    cclabel->prettyPrint(prettyPrintFile);
    
    cclabel->printEQAry(prettyPrintFile);
    prettyPrintFile<<endl;
    prettyPrintFile<<"ConnectPass2";
    cclabel->connectPass2(Connectness, cclabel->zeroFramedAry, cclabel->NonZeroNeighborAry);
    cclabel->prettyPrint(prettyPrintFile);
    cclabel->printEQAry(prettyPrintFile);
    cclabel->ManageEQAry(cclabel->EQAry, cclabel->newLabel);
    cclabel->printEQAry(prettyPrintFile);
    
    prettyPrintFile<<endl;
    prettyPrintFile<<"ConnectPass3";
    cclabel->connectPass3(cclabel->zeroFramedAry);
    
    cclabel->prettyPrint(prettyPrintFile);
    cclabel->printEQAry(prettyPrintFile);
    cclabel->printImage(labelFile);
    
    cclabel->printCCproperty(propertyFile);
    cclabel->drawBoxes(cclabel->zeroFramedAry, cclabel->CCproperty);
    cclabel->prettyPrint(prettyPrintFile);
    
    
    
    
    
//    for(int i = 1; i<cclabel.numRows*cclabel.numCols;i++)
//        cout<<cclabel.EQAry[i];
//    for(int i = 0;i<6;i++)
//        cout<<cclabel.NonZeroNeighborAry[i]<<endl;
//
    


    
    
    
    
    
    
    inFile.close();
    prettyPrintFile.close();
    propertyFile.close();
    labelFile.close();
}
