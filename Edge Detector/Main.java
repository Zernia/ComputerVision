import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.PrintWriter;
import java.util.Scanner;


public class Main {
	static int numRows;
	static int numCols;
	static int minVal;
	static int maxVal;
	static int newMax;
	static int newMin;
	static int newMaxVal= 0;
	static int newMinVal = 0;
	
	static int[][] mirrorFramedAry;
	static int[][] maskRobertRightDiag;
	static int[][] maskRobertLeftDiag;
	static int[][] maskSobelRightDiag;
	static int[][] maskSobelLeftDiag;
	
	static int[][] RobertRightDiag;
	static int[][] RobertLeftDiag;
	static int[][] SobelRightDiag;
	static int[][] SobelLeftDiag;
	
	static int[][] GradiantEdge;
	static int[][] edgeSum;
	
	static Scanner inFile;
	static PrintWriter RobertEdgeOut;
	static PrintWriter SobelEdgeOut;
	static PrintWriter GradiantEdgeOut;
	static PrintWriter prettyOut;
	static PrintWriter deBugOut;
	
	
	public static void main(String[] args){
		try{
//		    inFile = new Scanner(new FileReader("EdgeDetectors_Data.txt"));
//			RobertEdgeOut = new PrintWriter(new FileOutputStream("RobertEdgeOut.txt"));
//			SobelEdgeOut = new PrintWriter(new FileOutputStream("SobelEdgeOut.txt"));
//			GradiantEdgeOut = new PrintWriter(new FileOutputStream("GradientEdgeOut.txt"));
//			deBugOut = new PrintWriter(new FileOutputStream("deBugOut.txt"));
		    inFile = new Scanner(new FileReader(args[0]));
			RobertEdgeOut = new PrintWriter(new FileOutputStream(args[1]));
			SobelEdgeOut = new PrintWriter(new FileOutputStream(args[2]));
			GradiantEdgeOut = new PrintWriter(new FileOutputStream(args[3]));
			//prettyOut = new PrintWriter(new FileOutputStream(args[4]));
			deBugOut = new PrintWriter(new FileOutputStream(args[4]));
			
			numRows = numCols = minVal = maxVal = 0;

			numRows = inFile.nextInt();
			numCols = inFile.nextInt();
			minVal = inFile.nextInt();
			maxVal = inFile.nextInt();
			
			maskRobertRightDiag = new int[][]{{-1,1},{1,-1}};
			maskRobertLeftDiag = new int[][]{{1,-1},{-1,1}};
			maskSobelLeftDiag = new int[][]{{2,1,0},{1,0,-1},{0,-1,-2}};
			maskSobelRightDiag = new int[][]{{0,1,2},{-1,0,1},{-2,-1,0}};
			
			mirrorFramedAry = new int[numRows+2][numCols+2];
			RobertRightDiag = new int[numRows+2][numCols+2];
			RobertLeftDiag = new int[numRows+2][numCols+2];
			SobelRightDiag = new int[numRows+2][numCols+2];
			SobelLeftDiag = new int[numRows+2][numCols+2];
			GradiantEdge = new int[numRows+2][numCols+2];
			edgeSum = new int[numRows+2][numCols+2];
			
			loadImage(mirrorFramedAry);
			mirrorFramed(mirrorFramedAry);
			
//			for(int i = 0;i<numRows+2;i++){
//				for(int j = 0; j<numCols+2;j++){
//					System.out.print(mirrorFramedAry[i][j]+" ");
//				}
//				System.out.println();
//			}
			
			for(int i = 1; i <numRows+1;i++){
				for(int j = 1; j<numCols+1; j++){
					RobertRightDiag[i][j] = Math.abs(convoluteRobert(i,j,maskRobertRightDiag));
					RobertLeftDiag[i][j] = Math.abs(convoluteRobert(i,j,maskRobertLeftDiag));
                    SobelRightDiag[i][j] = Math.abs(convoluteSobel(i,j,maskSobelRightDiag));
                    SobelLeftDiag[i][j] = Math.abs(convoluteSobel(i,j,maskSobelLeftDiag));
                    GradiantEdge[i][j] = computeGradient(i,j);
					//System.out.print(GradiantEdge[i][j]+" ");
				}
				//System.out.println();
			}
			addTwoArys(RobertRightDiag, RobertLeftDiag, edgeSum);
			deBugOut.println("RobertRightDiag");
			//System.out.println("RobertRightDiag");
			prettyPrint(RobertRightDiag,deBugOut);
			deBugOut.println();
			deBugOut.println("RobertLeftDiag");
			//System.out.println("RobertLeftDiag");
			prettyPrint(RobertLeftDiag,deBugOut);
			
			RobertEdgeOut.println(numRows+" "+numCols+" "+newMin+" "+newMax);
			//System.out.println(numRows+" "+numCols+" "+newMin+" "+newMax);
			prettyPrint2(edgeSum,RobertEdgeOut);


			
			addTwoArys(SobelRightDiag, SobelLeftDiag, edgeSum);
			deBugOut.println("SobelRightDiag");
			//System.out.println("SobelRightDiag");
			prettyPrint(SobelRightDiag,deBugOut);
			deBugOut.println();
			deBugOut.println("SobelLeftDiag");
			//System.out.println("SobelLeftDiag");
			prettyPrint(SobelLeftDiag,deBugOut);
			
			SobelEdgeOut.println(numRows+" "+numCols+" "+newMin+" "+newMax);
			//System.out.println(numRows+" "+numCols+" "+newMin+" "+newMax);
			prettyPrint2(edgeSum,SobelEdgeOut);
			
			
			GradiantEdgeOut.println(numRows+" "+numCols+" "+newMinVal+" "+newMaxVal);			
			//System.out.println(numRows+" "+numCols+" "+newMinVal+" "+newMaxVal);

			prettyPrint2(GradiantEdge, GradiantEdgeOut);

	
			
			inFile.close();
			RobertEdgeOut.close();
			SobelEdgeOut.close();
			GradiantEdgeOut.close();
			deBugOut.close();
			
		}catch(FileNotFoundException e){
			e.printStackTrace();
		}
		

		

		
		
	}
	
	public static void loadImage(int[][] ary){
		for(int r = 1; r<numRows+1; r++){
			for(int c = 1; c<numCols+1;c++){
				mirrorFramedAry[r][c] = inFile.nextInt();
			}
		}
	}

	
	public static void mirrorFramed(int[][] ary){
		for(int r = 0 ; r<numRows+2;r++){
			mirrorFramedAry[r][0] = mirrorFramedAry[r][1];
			mirrorFramedAry[r][numCols+1] = mirrorFramedAry[r][numCols];
		}
		
		for(int c = 0; c<numCols+2; c++){
			mirrorFramedAry[0][c] = mirrorFramedAry[1][c];
			mirrorFramedAry[numRows+1][c] = mirrorFramedAry[numRows][c];
		}
	}
	
	public static int convoluteRobert(int i , int j, int[][]mask){
		int result = 0;
				result= mask[0][0]*mirrorFramedAry[i][j]+mask[0][1]*mirrorFramedAry[i][j+1]+mask[1][0]*mirrorFramedAry[i+1][j]+mask[1][1]*mirrorFramedAry[i+1][j+1];
				
			
		
		return result;
	}
	
	public static int convoluteSobel(int i, int j, int[][]mask){
		int result = 0;
		int ioffset = i-1;
		int joffset = j-1;
		for(int r =i-1;r<=i+1;r++){
			for(int c = j-1;c<=j+1;c++){
				result+= mask[r-ioffset][c-joffset]*mirrorFramedAry[r][c];
			}
		}
		return result; 
	}
	
	public static int computeGradient(int i , int j){
		
		double result = 0.0;
		
				result = Math.sqrt(Math.pow((double)(mirrorFramedAry[i][j]-mirrorFramedAry[i][j+1]),2)+Math.pow((double)(mirrorFramedAry[i][j]-mirrorFramedAry[i+1][j]), 2));
				//newMinVal = (int)result;
				//System.out.println(newMinVal);
				newMaxVal = newMaxVal<(int)result?(int)result : newMaxVal;
				newMinVal = newMinVal<(int)result?newMinVal:(int)result;
				

				return (int)result;
				
		
	}
	
	public static void addTwoArys(int[][] ary1, int[][] ary2, int[][] ary3){
		//newMax = 0;
		//newMin = 0;
		for(int i = 0;i<numRows+2;i++){
			for(int j = 0; j<numCols+2;j++){
				ary3[i][j] = ary1[i][j]+ary2[i][j];
				
			}
		}
		newMin = ary3[1][1];
		//newMax = ary3[1][1];

		for(int i = 1; i<numRows+1;i++){
			for(int j = 1; j<numCols+1;j++){
				newMax = newMax<ary3[i][j]?ary3[i][j]:newMax;
				newMin = newMin>ary3[i][j]?ary3[i][j]:newMin;
				//System.out.println("The new Min is"+newMin);

			}
		}
		
	}
	public static void prettyPrint(int[][] Ary, PrintWriter outFile){
		
		//outFile.println(numRows+" "+numCols+" "+minVal+" "+maxVal);
		//System.out.println(numRows+" "+numCols+" "+minVal+" "+newMax);

		for(int i = 0; i <numRows+2;i++ ){
			//System.out.println();
			outFile.println();

			for(int j = 0; j <numCols+2;j++){
				//System.out.print(Ary[i][j]+" ");
				outFile.print(Ary[i][j]+" ");
			}
		}
		//System.out.println();
		//System.out.println();

	}
	public static void prettyPrint2(int[][]Ary, PrintWriter outFile){
		//outFile.println(numRows+" "+numCols+" "+minVal+" "+maxVal);
		//System.out.println(numRows+" "+numCols+" "+minVal+" "+newMaxVal);
		for(int i = 1 ;i<numRows+1;i++){
			for(int j =1;j<numCols+1;j++){
				outFile.print(Ary[i][j]+" ");
				//System.out.print(Ary[i][j]+" ");
			}
			outFile.println();
			//System.out.println();
		}

	}

}
