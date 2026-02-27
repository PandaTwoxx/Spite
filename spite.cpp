#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stack>
#include <map>
#include <regex>
#include <sstream>
#include <cstdlib>

/**
 * SPITELANG: THE C# KILLER
 * v1.2.2 - Added global library search path and refined AOT logic.
 */

class SpiteCompiler {
public:
    std::string preprocess(std::string filename, std::vector<std::string>& loaded) {
        for (const auto& l : loaded) if (l == filename) return "";
        loaded.push_back(filename);

        std::ifstream file(filename);
        if (!file.is_open()) {
            // Check global lib path: ~/.spite/lib/filename
            const char* home_ptr = std::getenv("HOME");
            std::string home = home_ptr ? home_ptr : ".";
            std::string lib_path = home + "/.spite/lib/" + filename;
            file.open(lib_path);
            if (!file.is_open()) return "";
        }

        std::stringstream ss;
        std::string line;
        while (getline(file, line)) {
            std::regex imp_regex(R"(#import\s+\"([^\"]+)\")");
            std::smatch match;
            if (std::regex_search(line, match, imp_regex)) {
                ss << preprocess(match[1], loaded) << "\n";
            } else {
                size_t comm = line.find("//");
                if (comm != std::string::npos) line = line.substr(0, comm);
                ss << line << " ";
            }
        }
        return ss.str();
    }

    void compile_to_bin(std::string source, std::string out_name, size_t mem_size) {
        std::ofstream out("temp_build.cpp");
        out << "#include <iostream>\n#include <vector>\n#include <stack>\n";
        out << "int main() {\n";
        out << "    std::vector<int> a, b; std::stack<size_t> e; int P=0;\n";
        out << "    std::vector<int> v(" << mem_size << ", 0);\n";
        out << "    auto bury = [&](){ if(!a.empty()) v[((P % " << mem_size << ") + " << mem_size << ") % " << mem_size << "] = a.back(); };\n";
        out << "    auto exhume = [&](){ a.push_back(v[((P % " << mem_size << ") + " << mem_size << ") % " << mem_size << "]); };\n";
        
        for (size_t i = 0; i < source.length(); ++i) {
            unsigned char c = source[i];
            std::string op;
            if (c >= 0x80) {
                if ((c & 0xE0) == 0xC0) { op = source.substr(i, 2); i += 1; }
                else if ((c & 0xF0) == 0xE0) { op = source.substr(i, 3); i += 2; }
            } else { op = source[i]; }

            if (op == "@") {
                std::string name;
                while (i + 1 < source.length() && isalnum(source[++i])) name += source[i];
                out << "    " << name << ": ;\n";
            } else if (op == "∆") out << "    if(!a.empty()){ int t=a.back(); a.pop_back(); b.push_back(t+1); }\n";
            else if (op == "∇") out << "    if(!b.empty()){ int t=b.back(); b.pop_back(); a.push_back(t-1); }\n";
            else if (op == "ø") out << "    if(!a.empty()) a.push_back(a.back());\n";
            else if (op == "§") out << "    if(!a.empty()){ std::cout << (char)a.back(); a.pop_back(); }\n";
            else if (op == "#") out << "    if(!a.empty()){ std::cout << a.back(); a.pop_back(); }\n";
            else if (op == "⊞") out << "    bury();\n";
            else if (op == "⊟") out << "    exhume();\n";
            else if (op == "»") out << "    P++;\n";
            else if (op == "«") out << "    P--;\n";
            else if (op == "¿") out << "    if(!a.empty() && [&](){ int v=a.back(); a.pop_back(); return v; }() != 0) {\n";
            else if (op == "?") out << "    }\n";
            else if (op == "^") {
                std::string name;
                while (i + 1 < source.length() && isalnum(source[++i])) name += source[i];
                out << "    goto " << name << ";\n"; 
            } else if (isdigit(c)) {
                std::string num;
                while (i < source.length() && isdigit(source[i])) num += source[i++];
                out << "    a.push_back(" << num << ");\n";
                i--;
            }
        }
        out << "    return 0;\n}";
        out.close();
        std::string cmd = "g++ -O3 temp_build.cpp -o " + out_name;
        system(cmd.c_str());
    }
};

class SpiteVM {
private:
    std::vector<int> alpha, beta;
    std::stack<size_t> echo;
    std::vector<int> void_space;
    int P = 0;
    std::map<std::string, size_t> checkpoints;
    std::string program;
    size_t ip = 0;
    void scan_checkpoints() {
        for (size_t i = 0; i < program.length(); ++i) {
            if (program[i] == '@') {
                std::string name; size_t j = i + 1;
                while (j < program.length() && isalnum(program[j])) name += program[j++];
                checkpoints[name] = j;
            }
        }
    }
public:
    SpiteVM(std::string source, size_t memory_size) : program(source), void_space(memory_size, 0) { scan_checkpoints(); }
    void execute() {
        while (ip < program.length()) {
            std::string op; unsigned char c = program[ip];
            if (c >= 0x80) {
                if ((c & 0xE0) == 0xC0) { op = program.substr(ip, 2); ip += 1; }
                else if ((c & 0xF0) == 0xE0) { op = program.substr(ip, 3); ip += 2; }
            } else { op = program[ip]; }
            if (op == "∆") { if (!alpha.empty()) { int v = alpha.back(); alpha.pop_back(); beta.push_back(v + 1); } }
            else if (op == "∇") { if (!beta.empty()) { int v = beta.back(); beta.pop_back(); alpha.push_back(v - 1); } }
            else if (op == "ø") { if (!alpha.empty()) alpha.push_back(alpha.back()); }
            else if (op == "§") { if (!alpha.empty()) { std::cout << (char)alpha.back(); alpha.pop_back(); } }
            else if (op == "#") { if (!alpha.empty()) { std::cout << alpha.back(); alpha.pop_back(); } }
            else if (op == "⊞") { if (!alpha.empty()) void_space[((P % (int)void_space.size()) + void_space.size()) % void_space.size()] = alpha.back(); }
            else if (op == "⊟") { alpha.push_back(void_space[((P % (int)void_space.size()) + void_space.size()) % void_space.size()]); }
            else if (op == "»") { P++; } else if (op == "«") { P--; }
            else if (op == "¿") {
                int top = alpha.empty() ? 0 : alpha.back();
                if (!alpha.empty()) alpha.pop_back();
                if (top == 0) {
                    int depth = 1;
                    while (depth > 0 && ++ip < program.length()) {
                        if ((unsigned char)program[ip] == 0xC2 && (unsigned char)program[ip+1] == 0xBF) { depth++; ip++; }
                        else if (program[ip] == '?') depth--;
                    }
                }
            } else if (op == "^") {
                std::string name; while (ip + 1 < program.length() && isalnum(program[++ip])) name += program[ip];
                if (checkpoints.count(name)) ip = checkpoints[name] - 1;
            } else if (op == "↩") { if (!echo.empty()) { ip = echo.top(); echo.pop(); } }
            else if (isdigit(c)) {
                std::string num; while (ip < program.length() && isdigit(program[ip])) num += program[ip++];
                alpha.push_back(std::stoi(num)); ip--; 
            }
            ip++;
        }
    }
};

int main(int argc, char* argv[]) {
    if (argc < 2) { std::cout << "Usage:\n Run: spite <file.st> [mem]\n Build: spite -c <file.st> <out> [mem]" << std::endl; return 1; }
    std::string arg1 = argv[1];
    SpiteCompiler sc; std::vector<std::string> loaded;
    if (arg1 == "-c") {
        if (argc < 4) return 1;
        size_t mem = (argc >= 5) ? std::stoull(argv[4]) : 65536;
        std::string code = sc.preprocess(argv[2], loaded);
        sc.compile_to_bin(code, argv[3], mem);
    } else {
        size_t mem = (argc >= 3) ? std::stoull(argv[2]) : 65536;
        std::string code = sc.preprocess(argv[1], loaded);
        SpiteVM vm(code, mem); vm.execute();
    }
    return 0;
}