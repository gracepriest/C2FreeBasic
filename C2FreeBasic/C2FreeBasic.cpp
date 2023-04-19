// C2FreeBasic.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>
#include <sstream>
using namespace std;

//make a token list
enum TokenType 
{
	IDENTIFIER,
	PUNCTUATION,
	KEYWORD
};

struct Types
{
    std::string VOID = "void";
    std::string INT = "int";
    std::string CHAR = "char";
    std::string BOOL = "bool";
    std::string CONST = "const";
    std::string FLOAT = "float";
    std::string DOUBLE = "double";
    std::string UNSIGNED = "unsigned";
    std::string LONG = "long";
    std::string VECTOR2 = "Vector2";
    std::string VRSTEROMODE = "VrStereoMode";
    std::string SHADER = "Shader";
    std::string RAY = "Ray";
    std::string MATRIX = "Matrix";
    std::string FILEPATHLIST = "FilePathList";
    std::string IMAGE = "Image";
    std::string COLOR = "Color";
    std::string CAMERA2D = "Camera2D";
    std::string CAMERA3D = "Camera3D";
    std::string RENDERTEXTURE2D = "RenderTexture2D";
    std::string VRSTEREOCONFIG = "VrStereoConfig";
    std::string VRDEVICEINFO = "VrDeviceInfo";
    std::string TEXTURE2D = "Texture2D";
    std::string CAMERA = "Camera";
    std::string VECTOR3 = "Vector3";
    std::string TRACELOGCALLBACK = "TraceLogCallback";
    std::string LOADFILEDATACALLBACK = "LoadFileDataCallback";
    std::string SAVEFILEDATACALLBACK = "SaveFileDataCallback";
    std::string LOADFILETEXTCALLBACK = "LoadFileTextCallback";
    std::string SAVEFILETEXTCALLBACK = "SaveFileTextCallback";
    std::string RECTANGLE = "Rectangle";
    std::string FONT = "Font";
    std::string TEXTURECUBEMAP = "TextureCubemap";
    std::string  NPATCHINFO = "NPatchInfo";
    std::string GLYPHINFO = "GlyphInfo";
    std::string MESH = "Mesh";
    std::string MATERIALMAP = "MaterialMap";
    std::string MATERIAL = "Material";
    std::string TRANSFORM = "Transform";
    std::string BONEINFO = "BoneInfo";
    std::string  MODEL = "Model";
    std::string MODELANIMATION = "ModelAnimation";
    std::string RAYCOLLISION = "RayCollision";
    std::string BOUNDINGBOX = "BoundingBox";
    std::string WAVE = "Wave";
    std::string AUDIOSTREAM = "AudioStream";
    std::string SOUND = "Sound";
    std::string MUSIC = "Music";
    std::string VECTOR4 = "Vector4";
    std::string AUDIOCALLBACK = "AudioCallback";

    bool isType(string buff)
    {
        if (buff == VOID or
            buff == BOOL or
            buff == CHAR or
            buff == INT or
            buff == CONST or
            buff == DOUBLE or
            buff == FILEPATHLIST or
            buff == FLOAT or
            buff == LONG or
            buff == MATRIX or
            buff == RAY or
            buff == SHADER or
            buff == UNSIGNED or
            buff == VRSTEREOCONFIG or
            buff == IMAGE or
            buff == VECTOR2 or
            buff == COLOR or
            buff == CAMERA2D or
            buff == CAMERA3D or
            buff == RENDERTEXTURE2D or
            buff == VRSTEROMODE or
            buff == VRDEVICEINFO or
            buff == TEXTURE2D or
            buff == CAMERA or
            buff == VECTOR3 or
            buff == TRACELOGCALLBACK or
            buff == LOADFILEDATACALLBACK or
            buff == SAVEFILEDATACALLBACK or
            buff == LOADFILETEXTCALLBACK or 
            buff == SAVEFILETEXTCALLBACK or
            buff == RECTANGLE or 
            buff == FONT or
            buff == TEXTURECUBEMAP or 
            buff == GLYPHINFO or
            buff == NPATCHINFO or
            buff == MESH or
            buff == MATERIALMAP or
            buff == MATERIAL or
            buff == TRANSFORM or
            buff == BONEINFO or
            buff == MODEL or
            buff == MODELANIMATION or
            buff == RAY or
            buff == RAYCOLLISION or
            buff == BOUNDINGBOX or 
            buff == SOUND or
            buff == AUDIOSTREAM or 
            buff == MUSIC or
            buff == VECTOR4 or 
            buff == WAVE or
            buff == AUDIOCALLBACK)
        {
            return true;
        }
        return false;
    }
};
struct Punctuation
{
    char RIGHT_PARENTHESES = ')';
    char LEFT_PARENTHESES  = '(';
    char RIGHT_BRACKET     = ']';
    char LEFT_BRACKET      = '[';
    char COMMA             = ',';
    bool isPunctuation(char c)
    {
        if (c == RIGHT_PARENTHESES or
            c == LEFT_PARENTHESES or
            c == RIGHT_BRACKET or
            c == LEFT_BRACKET or
            c == COMMA)
        {
            return true;
        }
        return false;
    }
};

struct Token
{
	TokenType type;
	std::string value;
	//line if file where token is
	int line;
	//column where token is locate
	int column;

	Token(TokenType tokenType, const std::string& tokenValue, int tokenLine, int tokenColumn)
		: type(tokenType), value(tokenValue), line(tokenLine), column(tokenColumn) {}
};
//read in every letter, token matched than add to struct


std::vector<Token> ReadFile(std::ifstream& file)
{
    int lineNum = 1;
    int column = 0;
    char temp;
    std::string buffer;
    std::string line;
    Types types;
    Punctuation punctuation;
    std::vector<Token> tokens;

    while (!file.eof())
    {
        temp = file.get();
        column++;

        if (temp == '\n')
        {
            lineNum++;
            column = 0;
            continue;
        }
        if (temp == '/')
        {
            getline(file, line);
            lineNum++;
            column = 0;
            continue;
        }

        if (isspace(temp) || temp == ';')
        {
            if (!buffer.empty())
            {
                if (types.isType(buffer))
                {
                    tokens.emplace_back(Token(KEYWORD, buffer, lineNum, column - buffer.length()));
                }
                else
                {
                    tokens.emplace_back(Token(IDENTIFIER, buffer, lineNum, column - buffer.length()));
                }
                buffer.clear();
            }
        }
        else if (punctuation.isPunctuation(temp))
        {
            if (!buffer.empty())
            {
                if (types.isType(buffer))
                {
                    tokens.emplace_back(Token(KEYWORD, buffer, lineNum, column - buffer.length() - 1));
                }
                else
                {
                    tokens.emplace_back(Token(IDENTIFIER, buffer, lineNum, column - buffer.length() - 1));
                }
                buffer.clear();
            }
            tokens.emplace_back(Token(PUNCTUATION, std::string(1, temp), lineNum, column));
        }
        else
        {
            buffer += temp;
        }
    }

    // Add the last token if the buffer isn't empty
    if (!buffer.empty())
    {
        if (types.isType(buffer))
        {
            tokens.emplace_back(Token(KEYWORD, buffer, lineNum, column - buffer.length()));
        }
        else
        {
            tokens.emplace_back(Token(IDENTIFIER, buffer, lineNum, column - buffer.length()));
        }
    }

    // Print tokens for debugging purposes
    for (const auto& token : tokens)
    {
        std::cout << "Type: " << token.type << ", Value: " << token.value << ", Line: " << token.line << ", Column: " << token.column << std::endl;
    }
    return tokens;
}

size_t currentToken = 0;

Token consumeToken(const std::vector<Token>& tokens, TokenType expectedType, const std::string& expectedValue = "") {
    if (currentToken < tokens.size() && tokens[currentToken].type == expectedType &&
        (expectedValue.empty() || tokens[currentToken].value == expectedValue)) {

        return tokens[currentToken++];
    }
    else {
        // Error handling: raise an error or return an error token
        std::cout << "Error: Unexpected token at line " << tokens[currentToken].line << ", column " << tokens[currentToken].column << std::endl;
        exit(1);
    }
}

void parse_return_type(const std::vector<Token>& tokens) {
    consumeToken(tokens, KEYWORD);
}

void parse_identifier(const std::vector<Token>& tokens) {
    consumeToken(tokens, IDENTIFIER);
}

void parse_parameter_list(const std::vector<Token>& tokens) {
    if (tokens[currentToken].type == PUNCTUATION && tokens[currentToken].value == ")") {
        return;
    }

    while (true) {
        parse_return_type(tokens);
        if (tokens[currentToken].type != KEYWORD) {
            break;
        }
    }
    if (tokens[currentToken].type == IDENTIFIER)
    {
        parse_identifier(tokens);
    }
    if (tokens[currentToken].type == PUNCTUATION == ')')
    {
        return;
    }




    while (tokens[currentToken].type == PUNCTUATION && tokens[currentToken].value == ",") {
        consumeToken(tokens, PUNCTUATION, ",");
        parse_return_type(tokens);
        while (true) {
            parse_return_type(tokens);
            if (tokens[currentToken].type != KEYWORD) {
                break;
            }
        }
        if (tokens[currentToken].type == IDENTIFIER)
        {
            parse_identifier(tokens);
        }
        if(tokens[currentToken].type == PUNCTUATION == ')')
        {
            return;
        }
        
    }
    
}
std::string transform_return_type(const std::string& returnType) {
    if (returnType == "void") {
        return "sub";
    }
    else {
        return "function";
    }
}
void parse_and_transform_function_definition(const std::vector<Token>& tokens, std::stringstream& transformed_code) 
{

    //while (currentToken < tokens.size()) {
    //    std::string returnType = tokens[currentToken].value;
    //    transformed_code << transform_return_type(returnType) << " ";
    //    parse_return_type(tokens);

    //    parse_identifier(tokens);
    //    transformed_code << tokens[currentToken - 1].value;

    //    consumeToken(tokens, PUNCTUATION, "(");
    //    transformed_code << " (";

    //    if (tokens[currentToken].type != PUNCTUATION || tokens[currentToken].value != ")") {
    //        while (true) {
    //            std::string paramType = tokens[currentToken].value;
    //            while (true) {
    //                parse_return_type(tokens);
    //                if (tokens[currentToken].type != KEYWORD) {
    //                    break;
    //                }
    //            }
    //            if (tokens[currentToken].type == IDENTIFIER)
    //            {
    //                parse_identifier(tokens);
    //            }
    //            transformed_code << paramType << " " << tokens[currentToken - 1].value;

    //            if (tokens[currentToken].type == PUNCTUATION && tokens[currentToken].value == ",") {
    //                consumeToken(tokens, PUNCTUATION, ",");
    //                transformed_code << ", ";
    //            }
    //            else {
    //                break;
    //            }
    //        }
    //    }

    //    consumeToken(tokens, PUNCTUATION, ")");
    //    transformed_code << " )";
    //    transformed_code << endl << "end " << (returnType == "void" ? "sub" : "function");

    //    // Skip any whitespace or newlines between functions
    //    /*while (currentToken < tokens.size() &&  isspace((tokens[currentToken].type) || tokens[currentToken].type == '\n')){
    //        currentToken++;
    //    }*/
    //}
    while (currentToken < tokens.size()-1)
    {
        std::stringstream current_transformed_code;

        std::string returnType = tokens[currentToken].value;
        current_transformed_code << transform_return_type(returnType) << " ";
        while (true) {
            parse_return_type(tokens);
            if (tokens[currentToken].type != KEYWORD) {
                break;
            }
        }
        parse_identifier(tokens);
        current_transformed_code << tokens[currentToken - 1].value;

        consumeToken(tokens, PUNCTUATION, "(");
        current_transformed_code << " (";
        //parse_parameter_list(tokens);
        if (tokens[currentToken].type != PUNCTUATION || tokens[currentToken].value != ")") 
        {
            while (true) {
                std::string paramType;// = tokens[currentToken].value;
                while (true) {
                    if (tokens[currentToken].type != KEYWORD) {
                        break;
                    }
                    parse_return_type(tokens);
                    current_transformed_code << (paramType = tokens[currentToken-1].value) << " ";// << tokens[currentToken - 1].value;
                    
                }
                if (tokens[currentToken].type == IDENTIFIER)
                {
                    parse_identifier(tokens);
                    current_transformed_code << (paramType = tokens[currentToken - 1].value) << " ";// << tokens[currentToken - 1].value;
                }
                //if (tokens[currentToken].value != ")")
                //{
                //    //current_transformed_code << paramType << " " << tokens[currentToken - 1].value;
                //}
                //else
                //{
                //    //current_transformed_code << paramType<< " "<< tokens[currentToken-1].value;
                //}

                if (tokens[currentToken].type == PUNCTUATION && tokens[currentToken].value == ",") {
                    consumeToken(tokens, PUNCTUATION, ",");
                    current_transformed_code << ", ";
                }
                else {
                    break;
                }
            }
        }
        consumeToken(tokens, PUNCTUATION, ")");
        current_transformed_code << ")";
        current_transformed_code << endl << "end " << (returnType == "void" ? "sub" : "function") << endl;

        std::cout << current_transformed_code.str();
        transformed_code << current_transformed_code.str();
    }
}

int main()
{
    std::ifstream file;
    file.open("full.txt");
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file." << std::endl;
        return 1;
    }

    std::vector<Token> tokens = ReadFile(file); ReadFile(file);
   
    // Start parsing and transforming
    std::stringstream transformed_code;
    parse_and_transform_function_definition(tokens,transformed_code);

    // Output or save the transformed code
    //std::cout << "Transformed code:\n" << transformed_code.str() << std::endl;
    std::ofstream outfile("transformed_full.txt");

    outfile << transformed_code.str();
    outfile.close();
    file.close();
    return 0;
}

//void ReadFile(std::ifstream& file)
//{
//    int lineNum = 0;
//    int column = 0;
//    char temp;
//    std::string buffer;
//    Types types;
//    Punctuation punctuation;
//   // std::vector<Token> tokens;
//
//    while (!file.eof())
//    {
//        temp = file.get();
//        column++;
//
//        if (temp == '\n')
//        {
//            lineNum++;
//            continue;
//        }
//
//        if (isspace(temp))
//        {
//            if (!buffer.empty())
//            {
//                if (types.isType(buffer))
//                {
//                    tokens.emplace_back(Token(KEYWORD, buffer, lineNum, column));
//                }
//                else
//                {
//                    tokens.emplace_back(Token(IDENTIFIER, buffer, lineNum, column));
//                }
//                buffer.clear();
//            }
//        }
//        else if (punctuation.isPunctuation(temp))
//        {
//            if (!buffer.empty())
//            {
//                if (types.isType(buffer))
//                {
//                    tokens.emplace_back(Token(KEYWORD, buffer, lineNum, column));
//                }
//                else
//                {
//                    tokens.emplace_back(Token(IDENTIFIER, buffer, lineNum, column));
//                }
//                buffer.clear();
//            }
//            tokens.emplace_back(Token(PUNCTUATION, std::string(1, temp), lineNum, column));
//        }
//        else
//        {
//            buffer += temp;
//        }
//    }
//
//    // Add the last token if the buffer isn't empty
//    if (!buffer.empty())
//    {
//        if (types.isType(buffer))
//        {
//            tokens.emplace_back(Token(KEYWORD, buffer, lineNum, column));
//        }
//        else
//        {
//            tokens.emplace_back(Token(IDENTIFIER, buffer, lineNum, column));
//        }
//    }
//
//    // Print tokens for debugging purposes
//    for (const auto& token : tokens)
//    {
//        std::cout << "Type: " << token.type << ", Value: " << token.value << ", Line: " << token.line << ", Column: " << token.column << std::endl;
//    }
//}
//void ReadFile(std::ifstream& file)
//{
//    
//    int lineNum = 0;;
//    int column =0;
//    char temp;
//    string buffer;
//    Types types;
//    Punctuation punctuation;
//    
//
//
//    while (!file.eof())
//    {
//        temp = file.get();      //Reads one char from the file at a time
//        column++;               //the number of chars read in so far
//        buffer += temp;                        //Place the one char into a buffer
//
//        if (temp == ';')
//        {
//            buffer.pop_back();
//            continue;
//        }
//        if (temp == '\n')
//        {
//            lineNum++;
//            buffer.clear();
//            continue;
//           
//        }
//        else if (isspace(temp))
//        {
//            buffer.pop_back();         // Remove the whitespace character from the buffer
//            continue;
//        }
//        
//        if (types.isType(buffer)) {
//            tokens.emplace_back(Token(KEYWORD, buffer, lineNum, column));
//            buffer.clear();
//            continue;
//        }
//        if (punctuation.isPunctuation(temp) and buffer.empty())
//        {
//            tokens.emplace_back(Token(PUNCTUATION, string(1,temp), lineNum, column));
//            buffer.clear();
//            
//        }
//        else if (punctuation.isPunctuation(temp) and ispunct((char)buffer[0]))
//        {
//            tokens.emplace_back(Token(PUNCTUATION, string(1, temp), lineNum, column));
//            buffer.clear();
//           
//        }
//        else if(punctuation.isPunctuation(temp))
//        {
//            buffer.pop_back();
//            tokens.emplace_back(Token(IDENTIFIER, buffer, lineNum, column));
//            tokens.emplace_back(Token(PUNCTUATION, string(1, temp), lineNum, column));
//            buffer.clear();
//            
//        }
//        
//    }
//   // Print tokens for debugging purposes
//    for (const auto& token : tokens)
//    {
//        std::cout << "Type: " << token.type << ", Value: " << token.value << ", Line: " << token.line << ", Column: " << token.column << std::endl;
//    }
//}
        /*if (temp == punctuation.COMMA)
        {
            tokens.emplace_back(Token(PUNCTUATION, &punctuation.COMMA, lineNum, column));
            buffer.clear();

        }
        if (temp == punctuation.LEFT_BRACKET)
        {
            tokens.emplace_back(Token(PUNCTUATION, &punctuation.LEFT_BRACKET, lineNum, column));
            buffer.clear();
            continue;
        }
        if (temp == punctuation.RIGHT_BRACKET)
        {
            tokens.emplace_back(Token(PUNCTUATION, &punctuation.RIGHT_BRACKET, lineNum, column));
            buffer.clear();
            continue;
        }
        if (temp == punctuation.LEFT_PARENTHESES)
        {
            tokens.emplace_back(Token(PUNCTUATION, &punctuation.LEFT_PARENTHESES, lineNum, column));
            buffer.clear();
            continue;
        }
        if (temp == punctuation.RIGHT_PARENTHESES)
        {
            tokens.emplace_back(Token(PUNCTUATION, &punctuation.RIGHT_PARENTHESES, lineNum, column));
            buffer.clear();
            continue;
        }
        if (buffer == types.VOID)
        {
            tokens.emplace_back(Token(KEYWORD, types.VOID, lineNum, column));
            buffer.clear();
        }
        else if (buffer == types.BOOL)
        {
            tokens.emplace_back(Token(KEYWORD, types.BOOL, lineNum, column));
            buffer.clear();
        }
        else if (buffer == types.CHAR)
        {
            tokens.emplace_back(Token(KEYWORD, types.CHAR, lineNum, column));
            buffer.clear();
        }
        else if (buffer == types.INT)
        {
            tokens.emplace_back(Token(KEYWORD, types.INT, lineNum, column));
            buffer.clear();
        }
        else if (buffer == types.CONST)
        {
            tokens.emplace_back(Token(KEYWORD, types.CONST, lineNum, column));
            buffer.clear();
        }
        else if (buffer == types.DOUBLE)
        {
            tokens.emplace_back(Token(KEYWORD, types.DOUBLE, lineNum, column));
            buffer.clear();
        }
        else if (buffer == types.FILEPATHLIST)
        {
            tokens.emplace_back(Token(KEYWORD, types.FILEPATHLIST, lineNum, column));
            buffer.clear();
        }
        else if (buffer == types.FLOAT)
        {
            tokens.emplace_back(Token(KEYWORD, types.FLOAT, lineNum, column));
            buffer.clear();
        }
        else if (buffer == types.LONG)
        {
            tokens.emplace_back(Token(KEYWORD, types.LONG, lineNum, column));
            buffer.clear();
        }
        else if (buffer == types.MATRIX)
        {
            tokens.emplace_back(Token(KEYWORD, types.MATRIX, lineNum, column));
            buffer.clear();
        }
        else if (buffer == types.RAY)
        {
            tokens.emplace_back(Token(KEYWORD, types.RAY, lineNum, column));
            buffer.clear();
        }
        else if (buffer == types.SHADER)
        {
            tokens.emplace_back(Token(KEYWORD, types.SHADER, lineNum, column));
            buffer.clear();
        }
        else if (buffer == types.UNSIGNED)
        {
            tokens.emplace_back(Token(KEYWORD, types.UNSIGNED, lineNum, column));
            buffer.clear();
        }
        else if (buffer == types.VRSTEROCONFIG)
        {
            tokens.emplace_back(Token(KEYWORD, types.VRSTEROCONFIG, lineNum, column));
            buffer.clear();
        }*/
