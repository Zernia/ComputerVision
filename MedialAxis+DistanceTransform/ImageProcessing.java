import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.PrintWriter;
import java.util.Scanner;

public class ImageProcessing {
	public int numRows, numCols, minVal, maxVal;
	public int newMinVal, newMaxVal;
	
	public int zeroFramedAry[][];
	public int skeletonAry[][];
	
	public ImageProcessing(){
		
	}
	
	public void setZero(int Ary[][]){
		for(int i =0 ; i <numRows+2; i++){
			for(int j = 0 ; j<numCols+2; j++){
				Ary[i][j] = 0;
			}
		}
	}
	
	public void loadImage(Scanner inFile,int zeroFramedAry[][]){
		for(int i = 1; i <numRows+1; i++){
			for(int j = 1; j <numCols+1;j++){
				if(inFile.hasNextInt()){
				zeroFramedAry[i][j] = inFile.nextInt();
				}
			}
		}
		
	}
	
	public void compute8Distance(int zeroFramedAry[][], PrintWriter outFile1){
		firstPass8Distance(zeroFramedAry);
		//System.out.println("1st Pass distance transform" );
		outFile1.println("1st Pass distance transform" );

		prettyPrint(zeroFramedAry,outFile1);
		secondPass8Distance(zeroFramedAry);
		//System.out.println("2nd Pass distance transform" );
		outFile1.println("2nd Pass distance transform" );


		prettyPrint(zeroFramedAry,outFile1);

		
	}
	public void firstPass8Distance(int Ary[][]){
		int a =0, b =0, c=0, d = 0;
		for(int i =1; i <Ary.length-1;i++){//length-1||length-2
			for(int j = 1;j<Ary[0].length-1;j++){
				if(Ary[i][j]!=0){
				a = Ary[i-1][j-1];
				b = Ary[i-1][j];
				c = Ary[i-1][j+1];
				d = Ary[i][j-1];
				
				Ary[i][j] = Math.min(Math.min(Math.min(a,b),c),d)+1;
				//System.out.println(Ary[i][j]);
				}
				
			}
		}
	}
	public void secondPass8Distance(int Ary[][]){
		newMinVal = zeroFramedAry[1][1];
		newMaxVal = zeroFramedAry[1][1];
		int e = 0, f = 0, g = 0, h = 0;
		for(int i  = (Ary.length-2);i>0;i--){
			for(int j = (Ary[0].length-2); j>0;j--){
				if(Ary[i][j]!=0){
				e = Ary[i][j+1];
				f = Ary[i+1][j-1];
				g = Ary[i+1][j];
				h = Ary[i+1][j+1];
				Ary[i][j] = Math.min(Ary[i][j], Math.min(Math.min(Math.min(e,f),g),h)+1);
				}
				if(zeroFramedAry[i][j]<newMinVal){
					newMinVal = zeroFramedAry[i][j];
				}
				if(zeroFramedAry[i][j]>newMaxVal){
					newMaxVal = zeroFramedAry[i][j];
				}

			}
		}
	}
	
	public void firstPassExpension(int Ary[][]){
		for(int i = 1; i <Ary.length-1;i++){
			for(int j =1;j<Ary[0].length-1;j++){
				if(Ary[i][j] ==0){
					Ary[i][j] = 0;
					Ary[i][j] = Math.max(0, Math.max(Math.max(Math.max(Math.max(Math.max(Math.max(Math.max(Ary[i-1][j-1], Ary[i-1][j]),Ary[i-1][j+1]),Ary[i][j-1]),Ary[i][j+1]),Ary[i+1][j-1]),Ary[i+1][j]),Ary[i+1][j+1])-1);
					
					
				}
			}
		}
	}
	
	public void prettyPrint(int Ary[][],PrintWriter outFile){
		for(int i =1; i <Ary.length-1;i++){//length -1||length-2 = numberRow+1
			for(int j = 1; j <Ary[0].length-1;j++){
				    if(Ary[i][j]==0){
			
				    	//System.out.print("  ");
				    	outFile.print("  ");

				    }
					if(Ary[i][j]<10&&Ary[i][j]>0){
						//System.out.print("0"+Ary[i][j]+" ");
						outFile.print(" "+Ary[i][j]);

						
					}
					if(Ary[i][j]>=10){
						//System.out.print(Ary[i][j]+" ");
						outFile.print(Ary[i][j]);

					}

				}
			//System.out.println();
			outFile.println();

			}
	        //System.out.println();
	        outFile.println();

		}
	
	public void secondPassExpension(int Ary[][]){
		for(int i = Ary.length-2; i >=1;i-- ){
			for(int j = Ary[0].length-2; j>=1; j--){
				
				Ary[i][j] = Math.max(Ary[i][j], Math.max(Math.max(Math.max(Math.max(Math.max(Math.max(Math.max(Ary[i-1][j-1], Ary[i-1][j]),Ary[i-1][j+1]),Ary[i][j-1]),Ary[i][j+1]),Ary[i+1][j-1]),Ary[i+1][j]),Ary[i+1][j+1])-1);
				
			}
		}
		
	}
	
	public void skeletonExtraction(int Ary[][],int sAry[][], PrintWriter skeletonFile, PrintWriter outFile2){
		computeLocalMaxima(zeroFramedAry,skeletonAry, skeletonFile);
		outFile2.println("Local Maxima");
		prettyPrint(skeletonAry, outFile2);
		extractLocalMaxima(skeletonAry, skeletonFile);
		
	}
	
	public void computeLocalMaxima(int Ary[][], int sAry[][], PrintWriter skeletonFile){
		for(int i = 1; i<Ary.length-1;i++){
			for(int j = 1; j <Ary[0].length-1;j++){
				if((Ary[i][j]>0)&&(isLocalMaxima(zeroFramedAry, i, j)==1)){
					sAry[i][j] = zeroFramedAry[i][j];
					
				}
				else{
					sAry[i][j] =0;
				
				}
			}
		}
		
		//skeletonFile.close();
	
		
	}
	
	public int isLocalMaxima(int Ary[][], int i, int j){
		int max = 0;
		 max =Math.max(Math.max(Math.max(Math.max(Math.max(Math.max(Math.max(Ary[i-1][j-1], Ary[i-1][j]),Ary[i-1][j+1]),Ary[i][j-1]),Ary[i][j+1]),Ary[i+1][j-1]),Ary[i+1][j]),Ary[i+1][j+1]);
       if(Ary[i][j] >= max){
    	   return 1 ;
       }
       else return 0;
	}
	
	public void extractLocalMaxima(int sAry[][], PrintWriter skeletonFile){
		skeletonFile.println(numRows+" "+ numCols+" "+ newMinVal+" "+newMaxVal);
		for(int i = 1; i <sAry.length-1;i++){
			for(int j = 1; j <sAry[0].length-1;j++){
				if(sAry[i][j]>0){
					//System.out.println(i+" "+j+" "+sAry[i][j]);
					skeletonFile.println(i+" "+j+" "+sAry[i][j]);//subtract by 1 when outPut to skeletonFile?, I'm not sure.I don't.

				}
			}
		}
		skeletonFile.close();
	}
	
	public void skeletonExpansion(int Ary[][],PrintWriter outFile2, String skeletonFileName) throws FileNotFoundException{
		Scanner skeletonFile = new Scanner(new FileReader(skeletonFileName));
		setZero(zeroFramedAry);
		load(skeletonFile, zeroFramedAry);
        outFile2.println("1st Pass Expansion");
		firstPassExpension(zeroFramedAry);
		prettyPrint(zeroFramedAry, outFile2);
		outFile2.println("2nd pass Expansion");
		secondPassExpension(zeroFramedAry);
		prettyPrint(zeroFramedAry,outFile2);
		
	}
	public void load(Scanner inputFile, int Ary[][]){
		int i =0, j =0;
		numRows = inputFile.nextInt();
		numCols = inputFile.nextInt();
        newMinVal = inputFile.nextInt();
        newMaxVal = inputFile.nextInt();
		while(inputFile.hasNextInt()){
			i = inputFile.nextInt();
			j=inputFile.nextInt();
			Ary[i][j] = inputFile.nextInt();
			
			
		}
	}
	public void ary2File(int Ary[][], PrintWriter decompressFile){
		for(int i = 1; i <Ary.length-1;i++){
			for(int j = 1; j <Ary[0].length-1;j++){
				if(zeroFramedAry[i][j]>=1){
					//System.out.print("1"+" ");
					decompressFile.print("1"+" ");
				}
				else {
					//System.out.print("0"+" ");
					decompressFile.print("0"+" ");

				}
			}
			//System.out.println();
			decompressFile.println();
		}
	}

}
