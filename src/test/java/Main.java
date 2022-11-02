import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import com.codingame.gameengine.core.GameManager;
import com.codingame.gameengine.runner.MultiplayerGameRunner;
import com.codingame.gameengine.runner.simulate.GameResult;
import com.google.common.io.Files;
import com.google.gson.JsonObject;

import javassist.bytecode.analysis.Subroutine;

public class Main {
    public static void main(String[] args) throws IOException, InterruptedException {
    // agents compile setting
    	String agent1 = compileCpp("bot/main.cpp", "P1");
    	String agent2 = compileCpp("bot/main2.cpp", "P2"); 
    	
    	EAGeneBean gene1 = new EAGeneBean(100, 0f, "P1");
    	EAGeneBean gene2 = new EAGeneBean(100, 1f, "P2");

    	JsonObject jene1 = gene1.getJsonObject();
    	JsonObject jene2 = gene2.getJsonObject();
					
    // init data file
        FileWriter result_writer = new FileWriter("bot/result.txt");
		result_writer.write("");
		
	// Loop	
		long totaltime = 0; 
		for(int i=100, count=0; i<105; ++i, ++count) {
        	System.out.println(count);
        	long time = System.currentTimeMillis();
        	
        // write gene data to file
        	FileWriter P1_writer = new FileWriter("bot/data.json");
        	FileWriter P2_writer = new FileWriter("bot/data2.json");
        	P1_writer.flush();
			P1_writer.write(jene1.toString());
        	P1_writer.close();
        	
        	P2_writer.flush();
        	P2_writer.write(jene2.toString());
        	P2_writer.close();
        	
	// init gameRunner
		MultiplayerGameRunner gameRunner = new MultiplayerGameRunner();

		gameRunner.addAgent(agent1, "Player One");
		gameRunner.addAgent(agent2, "Player Two");
		gameRunner.setLeagueLevel(4);
		
	// get result
		GameResult result = gameRunner.simulate();
		result_writer.append("Game " + String.valueOf(count) + "\n");
		result_writer.append("Gene1: " + jene1 + "\n");
		result_writer.append("Gene2: " + jene2 + "\n");
		
	// result analyze and set new gene
		if(result.scores.get(0) > result.scores.get(1)) {
			result_writer.append("Player 1 win!\n");
			jene2.addProperty("Turn", i+1);
		}
		else if(result.scores.get(0) < result.scores.get(1)) {
			result_writer.append("Player 2 win!\n");
			jene1.addProperty("Turn", i+1);
		}
		else {
			result_writer.append("Tie!\n");
		}
  
//		result_writer.append(result.scores.toString() + "\n\n");
//		result_writer.append(result.outputs.toString() + "\n");
    	result_writer.append(result.errors.toString() + "\n");
    	result_writer.append(result.summaries.toString() + "\n");
//    	result_writer.append(result.gameParameters.toString()); //game seed
		
		time = System.currentTimeMillis() - time;
		totaltime += time;
		System.out.println("Execution Time: " + time + " ms");
	}
	
	result_writer.close();
	
	System.out.println("Total Execution Time: " + totaltime + " ms"); 	

/*
    	MultiplayerGameRunner gameRunner = new MultiplayerGameRunner();
    	gameRunner.addAgent(agent1, "Player One");
		gameRunner.addAgent(agent2, "Player Two");
		gameRunner.setLeagueLevel(3);
    	
// run game with launching web server     
         gameRunner.start();   
      // open browser
         Runtime rt = Runtime.getRuntime();
         String url = "http://localhost:8888/test.html";
         StringBuffer cmd = new StringBuffer();
         cmd.append( String.format("%s \"%s\"", "firefox", url) );
         rt.exec(new String[]{"sh", "-c", cmd.toString()} );
*/
}

private static String compileJava(String botFile) throws IOException, InterruptedException {
        File outFolder = Files.createTempDir();
        
        System.out.println("Compiling Boss.java... " + botFile);
        Process compileProcess = Runtime.getRuntime().exec(
            new String[] {
                "bash", "-c", "javac " + botFile + " -d " + outFolder.getAbsolutePath()
            }
        );
        compileProcess.waitFor();
        return "java -cp " + outFolder + " Player";
    }
    
    private static String compileCpp(String botFile, String playerName) throws IOException, InterruptedException {
        File outFolder = Files.createTempDir();
        
        System.out.println("Compiling Cpp " + botFile);
        Process compileProcess = Runtime.getRuntime().exec(
            "g++ " + botFile + " -o " + playerName
        );
        compileProcess.waitFor();
        System.out.println("./"+playerName);
        return "./" + playerName;
    }

//    private static String compileC(String botFile, String playerName) throws IOException, InterruptedException {
//        File outFolder = Files.createTempDir();
//        
//        FileReader fr = new FileReader(botFile);
//        BufferedReader br = new BufferedReader(fr);
//        System.out.println(br.readLine());
//        fr.close(); 
//        System.out.println("Compiling C " + botFile);
//        Process compileProcess = Runtime.getRuntime().exec(
//            "gcc -c " + botFile + " -o " + playerName + " -lm"
//        );
//        
//        compileProcess.waitFor();
//        
//        return "sudo -s ./" + playerName;
//    }
}
