public class LexicalErrorException extends Exception{
	String msg;
	public LexicalErrorException(String msg){
		super(msg);
		this.msg = msg;
	}
}