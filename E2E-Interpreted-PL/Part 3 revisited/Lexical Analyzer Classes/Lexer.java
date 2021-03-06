import java.io.*;


public class Lexer {
	private final int YY_BUFFER_SIZE = 512;
	private final int YY_F = -1;
	private final int YY_NO_STATE = -1;
	private final int YY_NOT_ACCEPT = 0;
	private final int YY_START = 1;
	private final int YY_END = 2;
	private final int YY_NO_ANCHOR = 4;
	private final int YY_BOL = 128;
	private final int YY_EOF = 129;

	public static void main(String[] args) throws IOException{
			String input = System.getProperty("user.home") + File.separator + "inp.txt";
                        String output = System.getProperty("user.home") + File.separator + "Token Stream.txt";
                        String symbol = System.getProperty("user.home") + File.separator + "Symbol Table.txt";
                        String literal = System.getProperty("user.home") + File.separator + "Literal Table.txt";
			String[][] symbolTable = new String[50][50];
	                String varRecentType = "null";
                        int dataTypeRecentLine = -1;
			int j = 0, flag = 0;
                        int funFlag = 0;
                        int defineFlag = 0;
			InputStream inp = new FileInputStream(input);
			File f = new File(output);
			PrintWriter out = new PrintWriter(f);
	                File symTable = new File(symbol);
			PrintWriter s = new PrintWriter(symTable);
	                File litTable = new File(literal);
			PrintWriter lit = new PrintWriter(litTable);
			Lexer lex = new Lexer(inp);
			Token tok;
			do{
				try{
					tok = lex.getNextToken();
					if(tok.id == -1){
					    do{
						tok = lex.getNextToken();
					    }while(!(tok.value.equals ("~")));
					}
                                        else if(tok.id == 1){
                                            symbolTable[1][j] = "%Main";
                                            j++;
                                        }
                                        else if(tok.id == 2){
                                            defineFlag = 1;
                                        }
                                        else if(tok.id == 12){
                                            funFlag = 1;
                                        }
					else if(tok.id == 8 || tok.id == 9){
                                            varRecentType = tok.value;
                                            dataTypeRecentLine = tok.line;
					}
					else if(tok.id == 34){
                                                if(funFlag == 1){
                                                    funFlag = 0;
                                                    symbolTable[1][j] = "%" + tok.value; 
                                                    j++;
                                                }
                                                else if(defineFlag == 1){
                                                    defineFlag = 0;
                                                    symbolTable[0][j] = "CONST";
                                                    symbolTable[1][j] = tok.value;
                                                    j++;
                                                }
                                                else{
                                                    for(int it = 0; it < j; it++){
                                                        if(tok.value.equals(symbolTable[1][it])){
                                                            flag = 1;
                                                            break;
                                                        }
                                                    }
                                                    if(flag == 0){
                                                        if(tok.line == dataTypeRecentLine){
                                                            symbolTable[0][j] = varRecentType;
                                                            symbolTable[1][j] = tok.value; 
                                                            j++;
                                                        }

                                                    }
                                                    flag = 0;
                                                }
					}
					else if(tok.id == 35){
					    lit.println("Character Literal: " + tok.value);
					}
					else if(tok.id == 36){
					    lit.println("String Literal: " + tok.value);
					}
					else if(tok.id == 37){
					    lit.println("Numeric Literal: " + tok.value);
					}
					if((tok.id != sym.tEOF) && (tok.id != -1)){
						out.println(tok.toString());
					}
				}
				catch(LexicalErrorException e){
					out.println(e.msg);
					break;
				}
			}while(tok.id != sym.tEOF);
			out.close();
			for(int iter = 0; iter < j; iter++){
                                if(symbolTable[0][iter] == null)
                                    s.println(symbolTable[1][iter]);
                                else
                                    s.println(symbolTable[0][iter] + "  " + symbolTable[1][iter]);
			}
			s.close();
	                lit.close();
	}
	private java.io.BufferedReader yy_reader;
	private int yy_buffer_index;
	private int yy_buffer_read;
	private int yy_buffer_start;
	private int yy_buffer_end;
	private char yy_buffer[];
	private int yyline;
	private boolean yy_at_bol;
	private int yy_lexical_state;

	public Lexer (java.io.Reader reader) {
		this ();
		if (null == reader) {
			throw (new Error("Error: Bad input stream initializer."));
		}
		yy_reader = new java.io.BufferedReader(reader);
	}

	public Lexer (java.io.InputStream instream) {
		this ();
		if (null == instream) {
			throw (new Error("Error: Bad input stream initializer."));
		}
		yy_reader = new java.io.BufferedReader(new java.io.InputStreamReader(instream));
	}

	private Lexer () {
		yy_buffer = new char[YY_BUFFER_SIZE];
		yy_buffer_read = 0;
		yy_buffer_index = 0;
		yy_buffer_start = 0;
		yy_buffer_end = 0;
		yyline = 0;
		yy_at_bol = true;
		yy_lexical_state = YYINITIAL;
	}

	private boolean yy_eof_done = false;
	private final int YYINITIAL = 0;
	private final int yy_state_dtrans[] = {
		0
	};
	private void yybegin (int state) {
		yy_lexical_state = state;
	}
	private int yy_advance ()
		throws java.io.IOException {
		int next_read;
		int i;
		int j;

		if (yy_buffer_index < yy_buffer_read) {
			return yy_buffer[yy_buffer_index++];
		}

		if (0 != yy_buffer_start) {
			i = yy_buffer_start;
			j = 0;
			while (i < yy_buffer_read) {
				yy_buffer[j] = yy_buffer[i];
				++i;
				++j;
			}
			yy_buffer_end = yy_buffer_end - yy_buffer_start;
			yy_buffer_start = 0;
			yy_buffer_read = j;
			yy_buffer_index = j;
			next_read = yy_reader.read(yy_buffer,
					yy_buffer_read,
					yy_buffer.length - yy_buffer_read);
			if (-1 == next_read) {
				return YY_EOF;
			}
			yy_buffer_read = yy_buffer_read + next_read;
		}

		while (yy_buffer_index >= yy_buffer_read) {
			if (yy_buffer_index >= yy_buffer.length) {
				yy_buffer = yy_double(yy_buffer);
			}
			next_read = yy_reader.read(yy_buffer,
					yy_buffer_read,
					yy_buffer.length - yy_buffer_read);
			if (-1 == next_read) {
				return YY_EOF;
			}
			yy_buffer_read = yy_buffer_read + next_read;
		}
		return yy_buffer[yy_buffer_index++];
	}
	private void yy_move_end () {
		if (yy_buffer_end > yy_buffer_start &&
		    '\n' == yy_buffer[yy_buffer_end-1])
			yy_buffer_end--;
		if (yy_buffer_end > yy_buffer_start &&
		    '\r' == yy_buffer[yy_buffer_end-1])
			yy_buffer_end--;
	}
	private boolean yy_last_was_cr=false;
	private void yy_mark_start () {
		int i;
		for (i = yy_buffer_start; i < yy_buffer_index; ++i) {
			if ('\n' == yy_buffer[i] && !yy_last_was_cr) {
				++yyline;
			}
			if ('\r' == yy_buffer[i]) {
				++yyline;
				yy_last_was_cr=true;
			} else yy_last_was_cr=false;
		}
		yy_buffer_start = yy_buffer_index;
	}
	private void yy_mark_end () {
		yy_buffer_end = yy_buffer_index;
	}
	private void yy_to_mark () {
		yy_buffer_index = yy_buffer_end;
		yy_at_bol = (yy_buffer_end > yy_buffer_start) &&
		            ('\r' == yy_buffer[yy_buffer_end-1] ||
		             '\n' == yy_buffer[yy_buffer_end-1] ||
		             2028/*LS*/ == yy_buffer[yy_buffer_end-1] ||
		             2029/*PS*/ == yy_buffer[yy_buffer_end-1]);
	}
	private java.lang.String yytext () {
		return (new java.lang.String(yy_buffer,
			yy_buffer_start,
			yy_buffer_end - yy_buffer_start));
	}
	private int yylength () {
		return yy_buffer_end - yy_buffer_start;
	}
	private char[] yy_double (char buf[]) {
		int i;
		char newbuf[];
		newbuf = new char[2*buf.length];
		for (i = 0; i < buf.length; ++i) {
			newbuf[i] = buf[i];
		}
		return newbuf;
	}
	private final int YY_E_INTERNAL = 0;
	private final int YY_E_MATCH = 1;
	private java.lang.String yy_error_string[] = {
		"Error: Internal error.\n",
		"Error: Unmatched input.\n"
	};
	private void yy_error (int code,boolean fatal) {
		java.lang.System.out.print(yy_error_string[code]);
		java.lang.System.out.flush();
		if (fatal) {
			throw new Error("Fatal Error.\n");
		}
	}
	private int[][] unpackFromString(int size1, int size2, String st) {
		int colonIndex = -1;
		String lengthString;
		int sequenceLength = 0;
		int sequenceInteger = 0;

		int commaIndex;
		String workString;

		int res[][] = new int[size1][size2];
		for (int i= 0; i < size1; i++) {
			for (int j= 0; j < size2; j++) {
				if (sequenceLength != 0) {
					res[i][j] = sequenceInteger;
					sequenceLength--;
					continue;
				}
				commaIndex = st.indexOf(',');
				workString = (commaIndex==-1) ? st :
					st.substring(0, commaIndex);
				st = st.substring(commaIndex+1);
				colonIndex = workString.indexOf(':');
				if (colonIndex == -1) {
					res[i][j]=Integer.parseInt(workString);
					continue;
				}
				lengthString =
					workString.substring(colonIndex+1);
				sequenceLength=Integer.parseInt(lengthString);
				workString=workString.substring(0,colonIndex);
				sequenceInteger=Integer.parseInt(workString);
				res[i][j] = sequenceInteger;
				sequenceLength--;
			}
		}
		return res;
	}
	private int yy_acpt[] = {
		/* 0 */ YY_NO_ANCHOR,
		/* 1 */ YY_NO_ANCHOR,
		/* 2 */ YY_NO_ANCHOR,
		/* 3 */ YY_NO_ANCHOR,
		/* 4 */ YY_NO_ANCHOR,
		/* 5 */ YY_NO_ANCHOR,
		/* 6 */ YY_NO_ANCHOR,
		/* 7 */ YY_NO_ANCHOR,
		/* 8 */ YY_NO_ANCHOR,
		/* 9 */ YY_NO_ANCHOR,
		/* 10 */ YY_NO_ANCHOR,
		/* 11 */ YY_NO_ANCHOR,
		/* 12 */ YY_NO_ANCHOR,
		/* 13 */ YY_NO_ANCHOR,
		/* 14 */ YY_NO_ANCHOR,
		/* 15 */ YY_NO_ANCHOR,
		/* 16 */ YY_NO_ANCHOR,
		/* 17 */ YY_NO_ANCHOR,
		/* 18 */ YY_NO_ANCHOR,
		/* 19 */ YY_NO_ANCHOR,
		/* 20 */ YY_NO_ANCHOR,
		/* 21 */ YY_NO_ANCHOR,
		/* 22 */ YY_NO_ANCHOR,
		/* 23 */ YY_NO_ANCHOR,
		/* 24 */ YY_NOT_ACCEPT,
		/* 25 */ YY_NO_ANCHOR,
		/* 26 */ YY_NO_ANCHOR,
		/* 27 */ YY_NO_ANCHOR,
		/* 28 */ YY_NO_ANCHOR,
		/* 29 */ YY_NO_ANCHOR,
		/* 30 */ YY_NO_ANCHOR,
		/* 31 */ YY_NO_ANCHOR,
		/* 32 */ YY_NO_ANCHOR,
		/* 33 */ YY_NO_ANCHOR,
		/* 34 */ YY_NO_ANCHOR,
		/* 35 */ YY_NO_ANCHOR,
		/* 36 */ YY_NO_ANCHOR,
		/* 37 */ YY_NO_ANCHOR,
		/* 38 */ YY_NO_ANCHOR,
		/* 39 */ YY_NO_ANCHOR,
		/* 40 */ YY_NO_ANCHOR,
		/* 41 */ YY_NO_ANCHOR,
		/* 42 */ YY_NO_ANCHOR,
		/* 43 */ YY_NO_ANCHOR,
		/* 44 */ YY_NO_ANCHOR,
		/* 45 */ YY_NOT_ACCEPT,
		/* 46 */ YY_NO_ANCHOR,
		/* 47 */ YY_NO_ANCHOR,
		/* 48 */ YY_NO_ANCHOR,
		/* 49 */ YY_NO_ANCHOR,
		/* 50 */ YY_NO_ANCHOR,
		/* 51 */ YY_NO_ANCHOR,
		/* 52 */ YY_NO_ANCHOR,
		/* 53 */ YY_NO_ANCHOR,
		/* 54 */ YY_NO_ANCHOR,
		/* 55 */ YY_NO_ANCHOR,
		/* 56 */ YY_NO_ANCHOR,
		/* 57 */ YY_NO_ANCHOR,
		/* 58 */ YY_NO_ANCHOR,
		/* 59 */ YY_NO_ANCHOR,
		/* 60 */ YY_NO_ANCHOR,
		/* 61 */ YY_NO_ANCHOR,
		/* 62 */ YY_NO_ANCHOR,
		/* 63 */ YY_NO_ANCHOR,
		/* 64 */ YY_NO_ANCHOR,
		/* 65 */ YY_NO_ANCHOR,
		/* 66 */ YY_NO_ANCHOR,
		/* 67 */ YY_NO_ANCHOR,
		/* 68 */ YY_NO_ANCHOR,
		/* 69 */ YY_NO_ANCHOR,
		/* 70 */ YY_NO_ANCHOR,
		/* 71 */ YY_NO_ANCHOR,
		/* 72 */ YY_NO_ANCHOR,
		/* 73 */ YY_NO_ANCHOR,
		/* 74 */ YY_NO_ANCHOR,
		/* 75 */ YY_NO_ANCHOR,
		/* 76 */ YY_NO_ANCHOR,
		/* 77 */ YY_NO_ANCHOR,
		/* 78 */ YY_NO_ANCHOR,
		/* 79 */ YY_NO_ANCHOR,
		/* 80 */ YY_NO_ANCHOR,
		/* 81 */ YY_NO_ANCHOR,
		/* 82 */ YY_NO_ANCHOR,
		/* 83 */ YY_NO_ANCHOR,
		/* 84 */ YY_NO_ANCHOR,
		/* 85 */ YY_NO_ANCHOR,
		/* 86 */ YY_NO_ANCHOR,
		/* 87 */ YY_NO_ANCHOR,
		/* 88 */ YY_NO_ANCHOR,
		/* 89 */ YY_NO_ANCHOR,
		/* 90 */ YY_NO_ANCHOR,
		/* 91 */ YY_NO_ANCHOR,
		/* 92 */ YY_NO_ANCHOR,
		/* 93 */ YY_NO_ANCHOR,
		/* 94 */ YY_NO_ANCHOR,
		/* 95 */ YY_NO_ANCHOR,
		/* 96 */ YY_NO_ANCHOR,
		/* 97 */ YY_NO_ANCHOR,
		/* 98 */ YY_NO_ANCHOR,
		/* 99 */ YY_NO_ANCHOR,
		/* 100 */ YY_NO_ANCHOR,
		/* 101 */ YY_NO_ANCHOR,
		/* 102 */ YY_NO_ANCHOR,
		/* 103 */ YY_NO_ANCHOR,
		/* 104 */ YY_NO_ANCHOR,
		/* 105 */ YY_NO_ANCHOR,
		/* 106 */ YY_NO_ANCHOR,
		/* 107 */ YY_NO_ANCHOR,
		/* 108 */ YY_NO_ANCHOR
	};
	private int yy_cmap[] = unpackFromString(1,130,
"24:9,38,39,24,38,39,24:18,38,24,23,24:4,41,27,28,20,31,25,32,19,33,22:10,26" +
",24,36,34,37,35,24,21:26,29,24,30,24:3,8,10,16,1,2,3,21,17,4,21:3,7,5,13,18" +
",14,9,6,12,11,15,21:4,24:3,40,24,0:2")[0];

	private int yy_rmap[] = unpackFromString(1,109,
"0,1,2,1,3,4,5,1:6,4:2,1:6,6,7,6,5:2,6:3,1,6:12,8,9,1,10,11,12,13,14,15,16,1" +
"7,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,4" +
"2,43,44,45,46,47,48,49,50,51,6,52,53,54,55,56,57,58,59,60,61,62,63,6,64,65," +
"66,67,68,69,70,71")[0];

	private int yy_nxt[][] = unpackFromString(72,42,
"1,2,43,64,100:2,46,100,105,107,100:2,108,48,100,79,65,100,66,3,4,100,5,6,44" +
",7,8,9,10,11,12,13,14,15,16,17,18,19,42:2,20,47,-1:43,100,80,100:16,-1:2,10" +
"0,81,-1:20,87:18,-1:2,87,-1:42,5,-1:20,24:22,25,24:15,-1,24:2,-1,100:18,-1:" +
"2,100,81,-1:20,100:4,69,100:13,-1:2,100,81,-1:57,42:2,-1:3,100:4,82,100:6,2" +
"1,100:6,-1:2,100,81,-1:60,29,-1,100:3,22,100:11,84,100:2,-1:2,100,81,-1:20," +
"45:38,-1,45:2,-1,100:10,23,100:7,-1:2,100,81,-1:20,100:4,26,100:13,-1:2,100" +
",81,-1:20,100:8,27,100:9,-1:2,100,81,-1:20,100:5,28,100:12,-1:2,100,81,-1:2" +
"0,100,30,100:16,-1:2,100,81,-1:20,100:11,31,100:6,-1:2,100,81,-1:20,100:4,3" +
"2,100:13,-1:2,100,81,-1:20,100:8,33,100:9,-1:2,100,81,-1:20,100,34,100:16,-" +
"1:2,100,81,-1:20,100,35,100:16,-1:2,100,81,-1:20,100:3,36,100:14,-1:2,100,8" +
"1,-1:20,100:5,37,100:12,-1:2,100,81,-1:20,100,38,100:16,-1:2,100,81,-1:20,1" +
"00,39,100:16,-1:2,100,81,-1:20,100,40,100:16,-1:2,100,81,-1:20,100:4,41,100" +
":13,-1:2,100,81,-1:20,100:3,49,100:8,83,100:5,-1:2,100,81,-1:20,100:7,50,10" +
"0:10,-1:2,100,81,-1:20,100:7,51,100:10,-1:2,100,81,-1:20,52,100:17,-1:2,100" +
",81,-1:20,100:10,53,100:7,-1:2,100,81,-1:20,100:12,54,100:5,-1:2,100,81,-1:" +
"20,100,55,100:16,-1:2,100,81,-1:20,100:4,56,100:13,-1:2,100,81,-1:20,100:11" +
",57,100:6,-1:2,100,81,-1:20,100:12,58,100:5,-1:2,100,81,-1:20,100:3,59,100:" +
"14,-1:2,100,81,-1:20,100:8,60,100:9,-1:2,100,81,-1:20,100:16,61,100,-1:2,10" +
"0,81,-1:20,100:10,62,100:7,-1:2,100,81,-1:20,100:12,63,100:5,-1:2,100,81,-1" +
":20,100:3,67,100:14,-1:2,100,81,-1:20,100:2,88,100:3,102,100:2,68,100:8,-1:" +
"2,100,81,-1:20,81:18,-1:2,81:2,-1:20,100:11,89,100:6,-1:2,100,81,-1:20,100:" +
"4,90,100:13,-1:2,100,81,-1:20,100:7,91,100:10,-1:2,100,81,-1:20,100:2,103,1" +
"00:15,-1:2,100,81,-1:20,100:8,92,100:9,-1:2,100,81,-1:20,100:3,95,100:14,-1" +
":2,100,81,-1:20,100:3,70,100:14,-1:2,100,81,-1:20,100:15,104,100:2,-1:2,100" +
",81,-1:20,100:4,71,100:13,-1:2,100,81,-1:20,100,72,100:16,-1:2,100,81,-1:20" +
",100:14,73,100:3,-1:2,100,81,-1:20,100:11,98,100:6,-1:2,100,81,-1:20,100:4," +
"74,100:13,-1:2,100,81,-1:20,100:8,75,100:9,-1:2,100,81,-1:20,100:15,76,100:" +
"2,-1:2,100,81,-1:20,100:13,77,100:4,-1:2,100,81,-1:20,100:3,78,100:14,-1:2," +
"100,81,-1:20,100:4,93,100:13,-1:2,100,81,-1:20,100:7,96,100:10,-1:2,100,81," +
"-1:20,100:3,97,100:14,-1:2,100,81,-1:20,100:11,99,100:6,-1:2,100,81,-1:20,1" +
"00:2,85,100:5,86,100:9,-1:2,100,81,-1:20,100:4,94,100:13,-1:2,100,81,-1:20," +
"100,101,100:16,-1:2,100,81,-1:20,100:7,106,100:10,-1:2,100,81,-1:19");

	public Token getNextToken ()
		throws java.io.IOException, LexicalErrorException {
		int yy_lookahead;
		int yy_anchor = YY_NO_ANCHOR;
		int yy_state = yy_state_dtrans[yy_lexical_state];
		int yy_next_state = YY_NO_STATE;
		int yy_last_accept_state = YY_NO_STATE;
		boolean yy_initial = true;
		int yy_this_accept;

		yy_mark_start();
		yy_this_accept = yy_acpt[yy_state];
		if (YY_NOT_ACCEPT != yy_this_accept) {
			yy_last_accept_state = yy_state;
			yy_mark_end();
		}
		while (true) {
			if (yy_initial && yy_at_bol) yy_lookahead = YY_BOL;
			else yy_lookahead = yy_advance();
			yy_next_state = YY_F;
			yy_next_state = yy_nxt[yy_rmap[yy_state]][yy_cmap[yy_lookahead]];
			if (YY_EOF == yy_lookahead && true == yy_initial) {

	return (new Token(sym.tEOF, yytext(), yyline+1));
			}
			if (YY_F != yy_next_state) {
				yy_state = yy_next_state;
				yy_initial = false;
				yy_this_accept = yy_acpt[yy_state];
				if (YY_NOT_ACCEPT != yy_this_accept) {
					yy_last_accept_state = yy_state;
					yy_mark_end();
				}
			}
			else {
				if (YY_NO_STATE == yy_last_accept_state) {
					throw (new Error("Lexical Error: Unmatched Input."));
				}
				else {
					yy_anchor = yy_acpt[yy_last_accept_state];
					if (0 != (YY_END & yy_anchor)) {
						yy_move_end();
					}
					yy_to_mark();
					switch (yy_last_accept_state) {
					case 0:
						{ }
					case -2:
						break;
					case 1:
						
					case -3:
						break;
					case 2:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -4:
						break;
					case 3:
						{return (new Token(sym.tDot, yytext(), yyline+1));}
					case -5:
						break;
					case 4:
						{return (new Token(sym.tStar, yytext(), yyline+1));}
					case -6:
						break;
					case 5:
						{return (new Token(sym.tNumLiteral, yytext(), yyline+1));}
					case -7:
						break;
					case 6:
						{throw new LexicalErrorException("ERROR: Invalid input at line " + String.valueOf(yyline+1));}
					case -8:
						break;
					case 7:
						{return (new Token(sym.tComma, yytext(), yyline+1));}
					case -9:
						break;
					case 8:
						{return (new Token(sym.tColon, yytext(), yyline+1));}
					case -10:
						break;
					case 9:
						{return (new Token(sym.tLparen, yytext(), yyline+1));}
					case -11:
						break;
					case 10:
						{return (new Token(sym.tRparen, yytext(), yyline+1));}
					case -12:
						break;
					case 11:
						{return (new Token(sym.tLSquare, yytext(), yyline+1));}
					case -13:
						break;
					case 12:
						{return (new Token(sym.tRSquare, yytext(), yyline+1));}
					case -14:
						break;
					case 13:
						{return (new Token(sym.tPlus, yytext(), yyline+1));}
					case -15:
						break;
					case 14:
						{return (new Token(sym.tMinus, yytext(), yyline+1));}
					case -16:
						break;
					case 15:
						{return (new Token(sym.tSlash, yytext(), yyline+1));}
					case -17:
						break;
					case 16:
						{return (new Token(sym.tEquals, yytext(), yyline+1));}
					case -18:
						break;
					case 17:
						{return (new Token(sym.tNotEquals, yytext(), yyline+1));}
					case -19:
						break;
					case 18:
						{return (new Token(sym.tLessThan, yytext(), yyline+1));}
					case -20:
						break;
					case 19:
						{return (new Token(sym.tGreaterThan, yytext(), yyline+1));}
					case -21:
						break;
					case 20:
						{return (new Token(-1, yytext(), yyline+1));}
					case -22:
						break;
					case 21:
						{return (new Token(sym.tEt, yytext(), yyline+1));}
					case -23:
						break;
					case 22:
						{return (new Token(sym.tSi, yytext(), yyline+1));}
					case -24:
						break;
					case 23:
						{return (new Token(sym.tOu, yytext(), yyline+1));}
					case -25:
						break;
					case 25:
						{return (new Token(sym.tStringLiteral, yytext(), yyline+1));}
					case -26:
						break;
					case 26:
						{return (new Token(sym.tFin, yytext(), yyline+1));}
					case -27:
						break;
					case 27:
						{return (new Token(sym.tCar, yytext(), yyline+1));}
					case -28:
						break;
					case 28:
						{return (new Token(sym.tPas, yytext(), yyline+1));}
					case -29:
						break;
					case 29:
						{return (new Token(sym.tCharLiteral, yytext(), yyline+1));}
					case -30:
						break;
					case 30:
						{return (new Token(sym.tVide, yytext(), yyline+1));}
					case -31:
						break;
					case 31:
						{return (new Token(sym.tDebut, yytext(), yyline+1));}
					case -32:
						break;
					case 32:
						{return (new Token(sym.tSinon, yytext(), yyline+1));}
					case -33:
						break;
					case 33:
						{return (new Token(sym.tEntier, yytext(), yyline+1));}
					case -34:
						break;
					case 34:
						{return (new Token(sym.tScanne, yytext(), yyline+1));}
					case -35:
						break;
					case 35:
						{return (new Token(sym.tArrete, yytext(), yyline+1));}
					case -36:
						break;
					case 36:
						{return (new Token(sym.tRenvoi, yytext(), yyline+1));}
					case -37:
						break;
					case 37:
						{return (new Token(sym.tDefinis, yytext(), yyline+1));}
					case -38:
						break;
					case 38:
						{return (new Token(sym.tDemarre, yytext(), yyline+1));}
					case -39:
						break;
					case 39:
						{return (new Token(sym.tAffiche, yytext(), yyline+1));}
					case -40:
						break;
					case 40:
						{return (new Token(sym.tTantque, yytext(), yyline+1));}
					case -41:
						break;
					case 41:
						{return (new Token(sym.tFonction, yytext(), yyline+1));}
					case -42:
						break;
					case 42:
						{ }
					case -43:
						break;
					case 43:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -44:
						break;
					case 44:
						{throw new LexicalErrorException("ERROR: Invalid input at line " + String.valueOf(yyline+1));}
					case -45:
						break;
					case 46:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -46:
						break;
					case 47:
						{throw new LexicalErrorException("ERROR: Invalid input at line " + String.valueOf(yyline+1));}
					case -47:
						break;
					case 48:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -48:
						break;
					case 49:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -49:
						break;
					case 50:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -50:
						break;
					case 51:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -51:
						break;
					case 52:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -52:
						break;
					case 53:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -53:
						break;
					case 54:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -54:
						break;
					case 55:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -55:
						break;
					case 56:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -56:
						break;
					case 57:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -57:
						break;
					case 58:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -58:
						break;
					case 59:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -59:
						break;
					case 60:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -60:
						break;
					case 61:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -61:
						break;
					case 62:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -62:
						break;
					case 63:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -63:
						break;
					case 64:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -64:
						break;
					case 65:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -65:
						break;
					case 66:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -66:
						break;
					case 67:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -67:
						break;
					case 68:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -68:
						break;
					case 69:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -69:
						break;
					case 70:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -70:
						break;
					case 71:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -71:
						break;
					case 72:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -72:
						break;
					case 73:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -73:
						break;
					case 74:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -74:
						break;
					case 75:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -75:
						break;
					case 76:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -76:
						break;
					case 77:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -77:
						break;
					case 78:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -78:
						break;
					case 79:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -79:
						break;
					case 80:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -80:
						break;
					case 81:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -81:
						break;
					case 82:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -82:
						break;
					case 83:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -83:
						break;
					case 84:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -84:
						break;
					case 85:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -85:
						break;
					case 86:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -86:
						break;
					case 87:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -87:
						break;
					case 88:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -88:
						break;
					case 89:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -89:
						break;
					case 90:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -90:
						break;
					case 91:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -91:
						break;
					case 92:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -92:
						break;
					case 93:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -93:
						break;
					case 94:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -94:
						break;
					case 95:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -95:
						break;
					case 96:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -96:
						break;
					case 97:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -97:
						break;
					case 98:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -98:
						break;
					case 99:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -99:
						break;
					case 100:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -100:
						break;
					case 101:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -101:
						break;
					case 102:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -102:
						break;
					case 103:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -103:
						break;
					case 104:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -104:
						break;
					case 105:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -105:
						break;
					case 106:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -106:
						break;
					case 107:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -107:
						break;
					case 108:
						{return (new Token(sym.tIdentifier, yytext(), yyline+1));}
					case -108:
						break;
					default:
						yy_error(YY_E_INTERNAL,false);
					case -1:
					}
					yy_initial = true;
					yy_state = yy_state_dtrans[yy_lexical_state];
					yy_next_state = YY_NO_STATE;
					yy_last_accept_state = YY_NO_STATE;
					yy_mark_start();
					yy_this_accept = yy_acpt[yy_state];
					if (YY_NOT_ACCEPT != yy_this_accept) {
						yy_last_accept_state = yy_state;
						yy_mark_end();
					}
				}
			}
		}
	}
}
