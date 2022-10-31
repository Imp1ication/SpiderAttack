import com.google.gson.JsonObject;

public class EAGeneBean {
	public int turn;
	public float otherFloat;
	public String otherString;
	
	public EAGeneBean(int t, float f, String s) {
		this.turn = t;
		this.otherFloat = f;
		this.otherString = s;
	}

	public JsonObject getJsonObject() {
		JsonObject jo = new JsonObject();	
		jo.addProperty("Turn", turn);
		jo.addProperty("Float", otherFloat);
		jo.addProperty("String", otherString);
		
		return jo;
	}
	
	
	
}
