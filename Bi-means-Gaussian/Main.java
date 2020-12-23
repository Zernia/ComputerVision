import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Scanner;

public class Main{
	static int numRows;
	static int numCols;
	static int minVal;
	static int maxVal;
	static int offSet;
	static int thrVal;
	static int[] histAry;
    static int maxHeight;
	static int[][] histGraph;
	static int[][] GaussGraph;
	static int[][] gapGraph;
	static int[] GaussAry;
    //static int bestThr;//不知道对不对 
	static PrintWriter outFile2;
	static int bestThrVal;
	static PrintWriter outFile3;

	public static void main(String[]args){
		try{
			Scanner inFile = new Scanner(new FileReader("BiGaussAutoThrSelection_data1.txt"));//记得改回来
			PrintWriter outFile1 = new PrintWriter(new FileOutputStream("outFile1"));//
		    outFile2 = new PrintWriter(new FileOutputStream("outFile2"));//
			outFile3 = new PrintWriter(new FileOutputStream("outFile3"));//
			
			numRows = numCols = minVal = maxVal = 0;
			
			numRows = inFile.nextInt();
			numCols = inFile.nextInt();
			minVal = inFile.nextInt();
			maxVal = inFile.nextInt();
			
			offSet = (maxVal-minVal)/10;
			thrVal = offSet;
			
			histAry = new int[maxVal+1];
			GaussAry = new int[maxVal+1];
			
			for(int i=0;i<=maxVal;i++){
				histAry[i]=0;
				GaussAry[i]=0;
			}
			loadHist(histAry,inFile);
			
			for(int i = 0;i<=maxVal;i++){
				maxHeight = Math.max(maxHeight,histAry[i]);
			}
			
			histGraph = new int[maxVal+1][maxHeight+1];
			GaussGraph = new int[maxVal+1][maxHeight+1];
			gapGraph = new int[maxVal+1][maxHeight+1];
			
			for(int i =0;i<=maxVal;i++){
				for(int j =0; j<=maxHeight;j++){
					histGraph[i][j]=0;
					GaussGraph[i][j]=0;
					gapGraph[i][j]=0;
				}
			}
			
			plotHistGraph(histGraph);
			
			outFile1.println("HistGraph");

			prettyPrint(histGraph, outFile1);
			
//			for(int i = 0;i<=maxVal;i++){
//			System.out.println();
//			for(int j = 0;j<maxHeight;j++){
//				System.out.print(GaussGraph[i][j]);
//			}
//		}
			
		    bestThrVal = biMeanGauss(thrVal);
		    
		    outFile1.println("The bestThrVal is "+bestThrVal );
		    outFile1.println();
		    outFile1.println("The GaussGraph is ");
		 //   for(int i = 0;i<GaussAry.length;i++){
		   // 	System.out.println(GaussAry[i]);
		    //}
//			for(int i = 0;i<GaussGraph.length;i++){
//			System.out.println();
//			for(int j = 0;j<GaussGraph[0].length;j++){
//				System.out.print(GaussGraph[i][j]);
//			}
//		}
      
		    prettyPrint(GaussGraph,outFile1);
		    outFile1.println("The gapGraph is ");
		    outFile1.println();
		    prettyPrint(gapGraph,outFile1);
			


			
            inFile.close();
			outFile1.close();
			outFile2.close();
			outFile3.close();
		}catch(IOException e){
			e.printStackTrace();
		}
		
	}
	
	public static void loadHist(int[] histAry, Scanner inFile){
		while(inFile.hasNext()){
			int index = inFile.nextInt();
			int value = inFile.nextInt();
			histAry[index]= value;
		}
	}
	
	public static void plotHistGraph(int [][] histGraph){
		for(int i =0;i<=maxVal;i++){
			histGraph[i][histAry[i]]=1;
		}
	}
	
	public static void prettyPrint(int[][]graph, PrintWriter outFile){
		for(int i=0;i<graph.length;i++){
			
			outFile.println();
			for(int j=0;j<graph[0].length;j++){
				if(graph[i][j]<=0){
			
					outFile.format(" ");
				}
				else{
					outFile.format("*");
				}
			}
		}
	}
	
	
	
	
	
	
	public static double computeMean(int leftIndex, int rightIndex){
		maxHeight = 0;
		double sum = 0.0;
		double numPixels = 0.0;
		
		
		for(int i = leftIndex; i<=rightIndex;i++){
			sum +=(i*histAry[i]);
			numPixels +=histAry[i];
			maxHeight = histAry[i] > maxHeight ? histAry[i] : maxHeight;
			//System.out.println("Mean is ");
		}
		//System.out.println(sum/numPixels);
		return sum/numPixels;
		
	}
	
	public static double computeVar(int leftIndex, int rightIndex, double mean){
		double sum = 0.0;
		double numPixels = 0.0;
		for(int i = leftIndex;i<=rightIndex;i++){
			sum+=((Math.pow((i - mean), 2)) *(histAry[i]));;
			numPixels +=histAry[i];
			
		}
		if(numPixels>0)
		    return sum/numPixels;
		return 0;
	}
	
	
	
	public static double modifiedGauss(int x, double mean,double var,int maxHeight){
	//System.out.println("The var is"+var);
		if(var==0.0)
			return 0.0;
		return maxHeight * (Math.exp(-1 * (Math.pow(x - mean, 2) / (2 * var))));
	}
	
	public static double fitGauss(int leftIndex, int rightIndex, int[]GaussAry, int [][]GaussGraph){
		double mean=0.0,var=0.0,sum=0.0,gVal=0.0,maxGval=0.0,temp=0.0;
		sum = 0.0;
		maxHeight = 0;//not sure
		mean = computeMean(leftIndex,rightIndex);
	    var = computeVar(leftIndex,rightIndex,mean);
		//System.out.println("The mean is"+mean);

		
		for(int i=leftIndex; i<=rightIndex;i++){
			gVal = modifiedGauss(i,mean,var,maxHeight);
			
			sum+=Math.abs(gVal-histAry[i]);
		
			GaussAry[i] = (int)gVal;

			GaussGraph[i][(int)gVal] = 1;
		}
	
		return sum;
	}
	
	public static void set1DZero(int[]ary){
		for(int i = 0; i<ary.length;i++){
			ary[i]=0;
		}
	}
	
	public static void set2DZero(int [][]ary){
		for(int i = 0;i<ary.length;i++){
			for(int j = 0; j<ary[0].length;j++){
				ary[i][j]=0;
			}
		}
	}
	
	public static int biMeanGauss(int thrVal){
		double sum1,sum2,total,minSumDiff;
		sum1=sum2=total= 0.0;
		int bestThr = thrVal;

		minSumDiff=99999.0;
	
		while(thrVal<(maxVal-offSet)){
		set1DZero(GaussAry);
		set2DZero(GaussGraph);
		set2DZero(gapGraph);
		
		sum1 = fitGauss(0,thrVal,GaussAry,GaussGraph);
		sum2 = fitGauss(thrVal,maxVal,GaussAry,GaussGraph);
		
		total = sum1+sum2;

		if(total <minSumDiff){
			minSumDiff = total;
			bestThr = thrVal;
		}

		thrVal++;
		outFile2.println("The thrVal is" +thrVal);
		outFile2.println();
		prettyPrint(GaussGraph,outFile2);
		plotGaps(histAry,GaussGraph,gapGraph);

		outFile3.println("The thrVal is" +thrVal);
		outFile3.println();

		prettyPrint(gapGraph,outFile3);
		}
	   // System.out.println(bestThr);
		return bestThr;
		
	}
	
	public static void plotGaps(int[]histAry,int[][]GaussGraph, int[][]gapGraph){
		int first=0,last=0;
		for(int i=0;i<=maxVal;i++){
		first = Math.min(histAry[i],GaussAry[i]);
		last = Math.max(histAry[i], GaussAry[i]);
//		for(int j = first; j<=last;j++){
//			gapGraph[i][first] = 1;
//		}
		while(first<=last){
			gapGraph[i][first] = 1;
			first++;
		}
		}
	}
	
	public static void bestThrPlot(int bestThrVal){
		
		double sum1=0.0,sum2=0.0;//not sure for what
		
		set1DZero(GaussAry);
		set2DZero(GaussGraph);
		set2DZero(gapGraph);
		//System.out.print(bestThrVal);
		sum1 = fitGauss(0,bestThrVal,GaussAry,GaussGraph);
		sum2 = fitGauss(bestThrVal,maxVal,GaussAry,GaussGraph);
		
		plotGaps(histAry,GaussGraph,gapGraph);
		//System.out.println(maxVal);
		//System.out.println(maxHeight);
//		for(int i=0;i<=maxVal;i++){
//			//System.out.println();
//			for(int j = 0;j<=maxHeight;j++){
//				System.out.print(GaussGraph[i][j]);
//			}
//		}
	}
	
}
