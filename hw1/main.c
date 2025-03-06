#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Token 類型列舉 */
typedef enum {
    TYPE_TOKEN,        // int
    MAIN_TOKEN,        // main
    IF_TOKEN,          // if
    ELSE_TOKEN,        // else
    WHILE_TOKEN,       // while
    ID_TOKEN,          // identifier
    LITERAL_TOKEN,     // integer literal
    LEFTPAREN_TOKEN,   // (
    RIGHTPAREN_TOKEN,  // )
    LEFTBRACE_TOKEN,   // {
    RIGHTBRACE_TOKEN,  // }
    SEMICOLON_TOKEN,   // ;
    ASSIGN_TOKEN,      // =
    EQUAL_TOKEN,       // ==
    GREATEREQUAL_TOKEN,// >=
    LESSEQUAL_TOKEN,   // <=
    GREATER_TOKEN,     // >
    LESS_TOKEN,        // <
    PLUS_TOKEN,        // +
    MINUS_TOKEN,       // -
    UNKNOWN_TOKEN,     // 無法識別
    END_OF_FILE_TOKEN  // 檔案結尾
} TokenType;

/* Token 結構 */
typedef struct {
    char lexeme[81];
    TokenType type;
} Token;

/* 輔助函數：回傳 token 類型的字串表示 */
const char* tokenTypeName(TokenType type) {
    switch(type) {
        case TYPE_TOKEN:         return "TYPE_TOKEN";
        case MAIN_TOKEN:         return "MAIN_TOKEN";
        case IF_TOKEN:           return "IF_TOKEN";
        case ELSE_TOKEN:         return "ELSE_TOKEN";
        case WHILE_TOKEN:        return "WHILE_TOKEN";
        case ID_TOKEN:           return "ID_TOKEN";
        case LITERAL_TOKEN:      return "LITERAL_TOKEN";
        case LEFTPAREN_TOKEN:    return "LEFTPAREN_TOKEN";
        case RIGHTPAREN_TOKEN:   return "RIGHTPAREN_TOKEN";
        case LEFTBRACE_TOKEN:    return "LEFTBRACE_TOKEN";
        case RIGHTBRACE_TOKEN:   return "RIGHTBRACE_TOKEN";
        case SEMICOLON_TOKEN:    return "SEMICOLON_TOKEN";
        case ASSIGN_TOKEN:       return "ASSIGN_TOKEN";
        case EQUAL_TOKEN:        return "EQUAL_TOKEN";
        case GREATEREQUAL_TOKEN: return "GREATEREQUAL_TOKEN";
        case LESSEQUAL_TOKEN:    return "LESSEQUAL_TOKEN";
        case GREATER_TOKEN:      return "GREATER_TOKEN";
        case LESS_TOKEN:         return "LESS_TOKEN";
        case PLUS_TOKEN:         return "PLUS_TOKEN";
        case MINUS_TOKEN:        return "MINUS_TOKEN";
        case UNKNOWN_TOKEN:      return "UNKNOWN_TOKEN";
        case END_OF_FILE_TOKEN:  return "EOF_TOKEN";
        default:                 return "UNKNOWN_TOKEN";
    }
}

/* 判斷空白字元 */
int is_whitespace(int c) {
    return (c == ' ' || c == '\n' || c == '\t');
}

/* 判斷符號（包含 {, }, (, ), ;） */
int is_sign(int c) {
    return (c == '{' || c == '}' || c == '(' || c == ')' || c == ';' || c == EOF || c == '\n' || c == '\t' || c == ' ');
}
int is_signA(int c) {
    return (c == '{' || c == '}' || c == '(' || c == ')' || c == ';');
}


/* 判斷是否為數字 */
int is_digit(int c) {
    return (c >= '0' && c <= '9');
}

/* 判斷是否為運算子中的字元 */
int is_operation(int c) {
    return (c == '+' || c == '-' || c == '>' || c == '<' || c == '=');
}

/* 判斷是否為英文字母或底線 */
int is_alpha(int c) {
    return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_');
}

int is_alphaA(int c) {
    return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'd') ||  (c >= 'f' && c <= 'h') ||  (c >= 'j' && c <= 'l') ||  (c >= 'n' && c <= 'v')  || c == '_');
}


/*
修改後的 getNextToken 函式：
將原本 switch–case 的 while 迴圈邏輯整合在此函式內，
當識別到一個 token 時就回傳，讓 main() 只需反覆呼叫此函式。
*/
Token getNextToken(FILE *fp) {
    int state = 0;
    int c;
    int bufIndex = 0;
    char buffer[100];
    Token token;
    
    memset(buffer, 0, sizeof(buffer));
    token.lexeme[0] = '\0';
    token.type = UNKNOWN_TOKEN;
    
    while (1) {
        switch (state) {
            case 0:
                c = fgetc(fp);
                if (c == EOF) {
                    state = 39;
                    break;
                }
                if (is_whitespace(c)) {
                    state = 0;
                } else if (c == '+') {
                    state = 1;
                } else if (c == '-') {
                    state = 2;
                } else if (c == '=') {
                    state = 3;
                } else if (c == '<') {
                    state = 6;
                } else if (c == '>') {
                    state = 9;
                } else if (is_digit(c)) {
                    bufIndex = 0;
                    buffer[bufIndex++] = (char)c;
                    buffer[bufIndex] = '\0';
                    state = 12;
                } else if (is_sign(c)) {
                    bufIndex = 0;
                    buffer[bufIndex++] = (char)c;
                    buffer[bufIndex] = '\0';
                    state = 14;
                } else if (c == 'e') {
                    bufIndex = 0;
                    buffer[bufIndex++] = (char)c;
                    buffer[bufIndex] = '\0';
                    state = 15;  // 可能為 else
                } else if (c == 'i') {
                    bufIndex = 0;
                    buffer[bufIndex++] = (char)c;
                    buffer[bufIndex] = '\0';
                    state = 18;  // 可能為 if 或 int
                } else if (c == 'm') {
                    bufIndex = 0;
                    buffer[bufIndex++] = (char)c;
                    buffer[bufIndex] = '\0';
                    state = 20;  // 可能為 main
                } else if (c == 'w') {
                    bufIndex = 0;
                    buffer[bufIndex++] = (char)c;
                    buffer[bufIndex] = '\0';
                    state = 23;  // 可能為 while
                } else if (is_alpha(c)) {
                    bufIndex = 0;
                    buffer[bufIndex++] = (char)c;
                    buffer[bufIndex] = '\0';
                    state = 37;  // 其他識別字
                }
                break;
                
            case 1:
                // PLUS_TOKEN
                strcpy(token.lexeme, "+");
                token.type = PLUS_TOKEN;
                state = 0;
                return token;
                
            case 2:
                // MINUS_TOKEN
                strcpy(token.lexeme, "-");
                token.type = MINUS_TOKEN;
                state = 0;
                return token;
                
            case 3:
                // 處理 '='，可能為 "=="
                c = fgetc(fp);
                if (c == '=') {
                    state = 4;
                } else {
                    state = 5;
                }
                break;
                
            case 4:
                strcpy(token.lexeme, "==");
                token.type = EQUAL_TOKEN;
                state = 0;
                return token;
                
            case 5:
                strcpy(token.lexeme, "=");
                token.type = ASSIGN_TOKEN;
                ungetc(c, fp);
                state = 0;
                return token;
                
            case 6:
                // 處理 '<'，可能為 "<="
                c = fgetc(fp);
                if (c == '=') {
                    state = 7;
                } else {
                    state = 8;
                }
                break;
                
            case 7:
                strcpy(token.lexeme, "<=");
                token.type = LESSEQUAL_TOKEN;
                state = 0;
                return token;
                
            case 8:
                strcpy(token.lexeme, "<");
                token.type = LESS_TOKEN;
                ungetc(c, fp);
                state = 0;
                return token;
                
            case 9:
                // 處理 '>'，可能為 ">="
                c = fgetc(fp);
                if (c == '=') {
                    state = 10;
                } else {
                    state = 11;
                }
                break;
                
            case 10:
                strcpy(token.lexeme, ">=");
                token.type = GREATEREQUAL_TOKEN;
                state = 0;
                return token;
                
            case 11:
                strcpy(token.lexeme, ">");
                token.type = GREATER_TOKEN;
                ungetc(c, fp);
                state = 0;
                return token;
                
            case 12:
                // 累積數字
                c = fgetc(fp);
                if (is_digit(c)) {
                    buffer[bufIndex++] = (char)c;
                    buffer[bufIndex] = '\0';
                    state = 12;
                } else if (is_alpha(c)) {
                    state = 99;
                } else if (is_sign(c) || is_operation(c)) {
                    buffer[bufIndex++] = (char)c;
                    buffer[bufIndex] = '\0';
                    state = 13;
                }
                break;
                
            case 13:
                strcpy(token.lexeme, buffer);
                token.type = LITERAL_TOKEN;
                ungetc(c, fp);
                state = 0;
                return token;
                
            case 14:
                if (c == '{') {
                    strcpy(token.lexeme, "{");
                    token.type = LEFTBRACE_TOKEN;
                } else if (c == '}') {
                    strcpy(token.lexeme, "}");
                    token.type = RIGHTBRACE_TOKEN;
                } else if (c == '(') {
                    strcpy(token.lexeme, "(");
                    token.type = LEFTPAREN_TOKEN;
                } else if (c == ')') {
                    strcpy(token.lexeme, ")");
                    token.type = RIGHTPAREN_TOKEN;
                } else if (c == ';') {
                    strcpy(token.lexeme, ";");
                    token.type = SEMICOLON_TOKEN;
                }
                state = 0;
                return token;
                
            case 15:
                // 處理以 'e' 開頭，可能為 "else"
                c = fgetc(fp);
                if (c == 'l') {
                    buffer[bufIndex++] = (char)c;
                    buffer[bufIndex] = '\0';
                    state = 16;
                } else if (is_alpha(c) || is_digit(c)) {
                    state = 37;
                } else if (is_sign(c) || is_operation(c)) {
                    buffer[bufIndex++] = (char)c;
                    buffer[bufIndex] = '\0';
                    state = 38;
                }
                break;
                
            case 16:
                c = fgetc(fp);
                if (c == 's') {
                    buffer[bufIndex++] = (char)c;
                    buffer[bufIndex] = '\0';
                    state = 17;
                } else if (is_alpha(c) || is_digit(c)) {
                    state = 37;
                } else if (is_sign(c) || is_operation(c)) {
                    buffer[bufIndex++] = (char)c;
                    buffer[bufIndex] = '\0';
                    state = 38;
                }
                break;
                
            case 17:
                c = fgetc(fp);
                if (c == 'e') {
                    buffer[bufIndex++] = (char)c;
                    buffer[bufIndex] = '\0';
                    state = 27;
                } else if (is_alpha(c) || is_digit(c)) {
                    state = 37;
                } else if (is_sign(c) || is_operation(c)) {
                    buffer[bufIndex++] = (char)c;
                    buffer[bufIndex] = '\0';
                    state = 38;
                }
                break;
                
            case 18:
                // 處理以 'i' 開頭，可能為 "if" 或 "int"
                c = fgetc(fp);
                if (c == 'f') {
                    buffer[bufIndex++] = (char)c;
                    buffer[bufIndex] = '\0';
                    state = 28;
                } else if (c == 'n') {
                    buffer[bufIndex++] = (char)c;
                    buffer[bufIndex] = '\0';
                    state = 19;
                } else if (is_alpha(c) || is_digit(c)) {
                    state = 37;
                } else if (is_sign(c) || is_operation(c)) {
                    buffer[bufIndex++] = (char)c;
                    buffer[bufIndex] = '\0';
                    state = 38;
                }
                break;
                
            case 19:
                // 處理 "int"，預期接 't'
                c = fgetc(fp);
                if (c == 't') {
                    buffer[bufIndex++] = (char)c;
                    buffer[bufIndex] = '\0';
                    state = 29;
                } else if (is_alpha(c) || is_digit(c)) {
                    state = 37;
                } else if (is_sign(c) || is_operation(c)) {
                    buffer[bufIndex++] = (char)c;
                    buffer[bufIndex] = '\0';
                    state = 38;
                }
                break;
                
            case 20:
                // 處理以 'm' 開頭，可能為 "main"，預期接 'a'
                c = fgetc(fp);
                if (c == 'a') {
                    buffer[bufIndex++] = (char)c;
                    buffer[bufIndex] = '\0';
                    state = 21;
                } else if (is_alpha(c) || is_digit(c)) {
                    state = 37;
                } else if (is_sign(c) || is_operation(c)) {
                    buffer[bufIndex++] = (char)c;
                    buffer[bufIndex] = '\0';
                    state = 38;
                }
                break;
                
            case 21:
                // 處理 "ma"，預期接 'i'
                c = fgetc(fp);
                if (c == 'i') {
                    buffer[bufIndex++] = (char)c;
                    buffer[bufIndex] = '\0';
                    state = 22;
                } else if (is_alpha(c) || is_digit(c)) {
                    state = 37;
                } else if (is_sign(c) || is_operation(c)) {
                    buffer[bufIndex++] = (char)c;
                    buffer[bufIndex] = '\0';
                    state = 38;
                }
                break;
                
            case 22:
                // 處理 "mai"，預期接 'n'
                c = fgetc(fp);
                if (c == 'n') {
                    buffer[bufIndex++] = (char)c;
                    buffer[bufIndex] = '\0';
                    state = 30;
                } else if (is_alpha(c) || is_digit(c)) {
                    state = 37;
                } else if (is_sign(c) || is_operation(c)) {
                    buffer[bufIndex++] = (char)c;
                    buffer[bufIndex] = '\0';
                    state = 38;
                }
                break;
                
            case 23:
                // 處理以 'w' 開頭，可能為 "while"
                c = fgetc(fp);
                if (c == 'h') {
                    buffer[bufIndex++] = (char)c;
                    buffer[bufIndex] = '\0';
                    state = 24;
                } else if (is_alpha(c) || is_digit(c)) {
                    state = 37;
                } else if (is_sign(c) || is_operation(c)) {
                    buffer[bufIndex++] = (char)c;
                    buffer[bufIndex] = '\0';
                    state = 38;
                }
                break;
                
            case 24:
                c = fgetc(fp);
                if (c == 'i') {
                    buffer[bufIndex++] = (char)c;
                    buffer[bufIndex] = '\0';
                    state = 25;
                } else if (is_alpha(c) || is_digit(c)) {
                    state = 37;
                } else if (is_sign(c) || is_operation(c)) {
                    buffer[bufIndex++] = (char)c;
                    buffer[bufIndex] = '\0';
                    state = 38;
                }
                break;
                
            case 25:
                c = fgetc(fp);
                if (c == 'l') {
                    buffer[bufIndex++] = (char)c;
                    buffer[bufIndex] = '\0';
                    state = 26;
                } else if (is_alpha(c) || is_digit(c)) {
                    state = 37;
                } else if (is_sign(c) || is_operation(c)) {
                    buffer[bufIndex++] = (char)c;
                    buffer[bufIndex] = '\0';
                    state = 38;
                }
                break;
                
            case 26:
                c = fgetc(fp);
                if (c == 'e') {
                    buffer[bufIndex++] = (char)c;
                    buffer[bufIndex] = '\0';
                    state = 31;
                } else if (is_alpha(c) || is_digit(c)) {
                    state = 37;
                } else if (is_sign(c) || is_operation(c)) {
                    buffer[bufIndex++] = (char)c;
                    buffer[bufIndex] = '\0';
                    state = 38;
                }
                break;
                
            case 27:
                c = fgetc(fp);
                if (is_sign(c) || is_operation(c)) {
                    buffer[bufIndex++] = (char)c;
                    buffer[bufIndex] = '\0';
                    state = 32;
                } else if (is_alpha(c) || is_digit(c)) {
                    state = 37;
                }
                break;
                
            case 28:
                c = fgetc(fp);
                if (is_sign(c) || is_operation(c)) {
                    buffer[bufIndex++] = (char)c;
                    buffer[bufIndex] = '\0';
                    state = 33;
                } else if (is_alpha(c) || is_digit(c)) {
                    state = 37;
                }
                break;
                
            case 29:
                c = fgetc(fp);
                if (is_sign(c) || is_operation(c)) {
                    buffer[bufIndex++] = (char)c;
                    buffer[bufIndex] = '\0';
                    state = 34;
                } else if (is_alpha(c) || is_digit(c)) {
                    state = 37;
                }
                break;
                
            case 30:
                c = fgetc(fp);
                if (is_sign(c) || is_operation(c)) {
                    buffer[bufIndex++] = (char)c;
                    buffer[bufIndex] = '\0';
                    state = 35;
                } else if (is_alpha(c) || is_digit(c)) {
                    state = 37;
                }
                break;
                
            case 31:
                c = fgetc(fp);
                if (is_sign(c) || is_operation(c)) {
                    buffer[bufIndex++] = (char)c;
                    buffer[bufIndex] = '\0';
                    state = 36;
                } else if (is_alpha(c) || is_digit(c)) {
                    state = 37;
                }
                break;
                
            case 32:
                // 識別出 "else"
                strcpy(token.lexeme, "else");
                token.type = ELSE_TOKEN;
                ungetc(c, fp);
                state = 0;
                return token;
                
            case 33:
                // 識別出 "if"
                strcpy(token.lexeme, "if");
                token.type = IF_TOKEN;
                ungetc(c, fp);
                state = 0;
                return token;
                
            case 34:
                // 識別出 "int"
                strcpy(token.lexeme, "int");
                token.type = TYPE_TOKEN;
                ungetc(c, fp);
                state = 0;
                return token;
                
            case 35:
                // 識別出 "main"
                strcpy(token.lexeme, "main");
                token.type = MAIN_TOKEN;
                ungetc(c, fp);
                state = 0;
                return token;
                
            case 36:
                // 識別出 "while"
                strcpy(token.lexeme, "while");
                token.type = WHILE_TOKEN;
                ungetc(c, fp);
                state = 0;
                return token;
                
            case 37:
                c = fgetc(fp);
                if (c != EOF && (is_alphaA(c) || is_digit(c))) {
                    buffer[bufIndex++] = (char)c;
                    buffer[bufIndex] = '\0';
                    state = 37;
                } else if (c != EOF && (is_sign(c) || is_operation(c))) {
                    buffer[bufIndex++] = (char)c;
                    buffer[bufIndex] = '\0';
                    state = 38;
                } else {
                    state = 38;
                }
                break;
                
            case 38:
                strcpy(token.lexeme, buffer);
                token.type = ID_TOKEN;
                ungetc(c, fp);
                state = 0;
                return token;
                
            case 39:
                strcpy(token.lexeme, "EOF");
                token.type = END_OF_FILE_TOKEN;
                state = 0;
                return token;
                

                
            default:
                strcpy(token.lexeme, "error");
                token.type = UNKNOWN_TOKEN;
                return token;
        }
    }
}

int main() {
    FILE *fp = fopen("text.c", "r");
    if (fp == NULL) {
        fprintf(stderr, "無法開啟檔案 text.c\n");
        return 1;
    }
    
    Token token;
    while (1) {
        token = getNextToken(fp);
        if (token.type == END_OF_FILE_TOKEN)
            break;
        printf("%s: %s\n", token.lexeme, tokenTypeName(token.type));
    }
    
    fclose(fp);
    return 0;
}