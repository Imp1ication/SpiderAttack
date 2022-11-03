import com.google.gson.JsonObject;

public class EAGene {
	private JsonObject jene_;
	
/* Threat System */
	private float monsHealth_, monsTarget_, monsShield_, monsControl_;
	private float heroControl_, heroWind_, heroNear_;

/* Game Manager */
	private int atkTurn_, atkMana_;
	private float defThreat;
	
	public EAGene(String geneJsonFile) {
        jene_ = new JsonObject();

		
	}
	
}
