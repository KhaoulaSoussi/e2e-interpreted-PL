
public class Token{
	int id;
	String value;
	int line;
	
	public Token(int id, String value, int line){
		this.id = id;
		this.value = value;
		this.line = line;
	}
	public String toString(){
		return "Line " + line + " Token " + id + ": " + value ;
	}
}
