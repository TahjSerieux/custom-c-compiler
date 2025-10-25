#include <string>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Lexer.hpp"
#include "Parser.hpp"
#include "AST.hpp"
#include "Assembly.hpp"
int main(int argc, char* argv[]){
    if(argc<2){
        std::cout<<"Source file was not provided";
        return(-1);
    }else{
        std::cout<<"Processing Source File\n";
        std::string sourceFile =  argv[1];
        std::cout<<sourceFile<<'\n';

        //Getting the file name to construct output file name for command
        std::string command = "gcc -E -P " +  sourceFile;
        size_t lastDot = sourceFile.find_last_of('.');
        if(lastDot == std::string::npos){
            std::cout<<"File has no extension";
            return(-1);    
        }
        //replacing .c with .i of file name
        std::string fileName = sourceFile.substr(0,lastDot);
        std::string preprocessFileName =  fileName + ".i";
        //constructing command to preprocess .c file
        command += " -o " + preprocessFileName;
        std::system(command.c_str());
        std::cout<<"Created Preprocessed File: "<<preprocessFileName<<'\n';
        //check if Preprocessed file is open
        std::ifstream inputeFile(preprocessFileName);
        if(!inputeFile.is_open()){
            std::cerr << "Error: Could not open " << preprocessFileName << '\n';
            return(-1);
        }

        std::stringstream buffer;
        buffer<<inputeFile.rdbuf();
        std::string fileContent = buffer.str();
        inputeFile.close();

        // std::cout<<"File's content: \n"<<fileContent<<'\n\n';
        
        Lexer lexer{};
        try
        {
            lexer.tokenize(fileContent);
            std::cout<<"Tokens:\n";
            lexer.printTokens();
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            if(remove(preprocessFileName.c_str()) == 0){
    
                std::cout<<"Deleted: "<<preprocessFileName<<'\n';
            }else{
                std::perror("Error deleting file");
                    std::cout << "Could not delete: " << preprocessFileName << '\n';
            }
            std::cout<<"Exiting as a failure\n";
            return(-1);
        }
        Parser parser{lexer.getTokens()};
        // ProgramNode* Program = nullptr;
        try
        {
            AST ast{parser.parseProgram()};
            std::cout<<"----------------\nPARSE SUCCESSFUL\n----------------\n";
            
            ast.PrettyPrint();
            TackyGenerator tackyGenerator{};
            TackyProgram* tackyProgram = tackyGenerator.convertProgram(&ast);
            tackyProgram->prettyPrint();
            std::cout<<"-------------------------------------------------------------------------------\n";
            IRTree intermidate{};
            intermidate.transformFromTacky(tackyProgram);
            intermidate.replacePseudoOperands();
            intermidate.filePrint(fileName);
        }
        // IRTree intermidate{ast};
        // intermidate.transform();
        // // TackyProgram* tackyProgram =  intermidate.
        // // intermidate.prettyPrint();
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            if(remove(preprocessFileName.c_str()) == 0){
                
                std::cout<<"Deleted: "<<preprocessFileName<<'\n';
            }else{
                std::perror("Error deleting file");
                std::cout << "Could not delete: " << preprocessFileName << '\n';
            }
            std::cout<<"Exiting as a failure\n";
            return(-1);
        }

        // // ------------------------------------------------------------------------------------------------------------------------------------------
        // if(remove(preprocessFileName.c_str()) == 0){

        //     std::cout<<"Deleted: "<<preprocessFileName<<'\n';
        // }else{
        //     std::perror("Error deleting file");
        //         std::cout << "Could not delete: " << preprocessFileName << '\n';
        // }

        
        
    }
    std::cout<<"Exiting as success\n";
    return(0);

}