import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.PrintWriter;
import java.util.Scanner;

public class Main {
	static Scanner inFile;
	static PrintWriter outFile1;
	static PrintWriter outFile2;
	static PrintWriter skeletonFile;
	static PrintWriter decompressFile;
	
	public static void main(String args[]){
		try{

		inFile = new Scanner(new FileReader(args[0]));
		//inFile = new Scanner(new FileReader("DistLocalMaximaDeCompress_image1.txt"));
		//outFile1 = new PrintWriter(new FileOutputStream("OutFile1") );
		//outFile2 = new PrintWriter(new FileOutputStream("OutFile2") );
		outFile1 = new PrintWriter(new FileOutputStream(args[1]) );
		outFile2 = new PrintWriter(new FileOutputStream(args[2]) );
		
		String skeletonFileName = args[1].replace(".txt", "_skeleton.txt");
		skeletonFile = new PrintWriter(new FileOutputStream(skeletonFileName));
		String decompressFileName = args[1].replaceAll(".txt", "_decompressed.txt");
		decompressFile = new PrintWriter(new FileOutputStream(decompressFileName));
		
		ImageProcessing imageprocessing = new ImageProcessing();
		imageprocessing.numRows = inFile.nextInt();
		imageprocessing.numCols = inFile.nextInt();
		imageprocessing.minVal = inFile.nextInt();
		imageprocessing.maxVal = inFile.nextInt();
		//System.out.print(imageprocessing.maxVal);
		
		imageprocessing.zeroFramedAry = new int[imageprocessing.numRows+2][imageprocessing.numCols+2];
		imageprocessing.skeletonAry = new int[imageprocessing.numRows+2][imageprocessing.numCols+2];
		
		imageprocessing.setZero(imageprocessing.zeroFramedAry);
		imageprocessing.setZero(imageprocessing.skeletonAry);
		imageprocessing.loadImage(inFile, imageprocessing.zeroFramedAry);
		imageprocessing.compute8Distance(imageprocessing.zeroFramedAry, outFile1);

		imageprocessing.skeletonExtraction(imageprocessing.zeroFramedAry, imageprocessing.skeletonAry, skeletonFile, outFile1);
		
		imageprocessing.skeletonExpansion(imageprocessing.zeroFramedAry, outFile2, skeletonFileName);
		
		decompressFile.println(imageprocessing.numRows+" "+imageprocessing.numCols+" "+imageprocessing.newMinVal+" "+imageprocessing.newMaxVal );
		imageprocessing.ary2File(imageprocessing.zeroFramedAry, decompressFile);//the only different is the newMaxVal;the trace is in secondepass

		inFile.close();
		outFile1.close();
		outFile2.close();
		//skeletonFile.close();
		decompressFile.close();
		}catch(FileNotFoundException e){
			e.printStackTrace();
		}

		


		
	}

}
