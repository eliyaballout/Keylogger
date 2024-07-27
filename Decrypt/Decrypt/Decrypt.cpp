#include <iostream>
#include <fstream>
#include <string>
#include <sstream>



std::string Decrypt(const std::string& data, const std::string& key) {
	std::string decrypted = data;
	for (size_t i = 0; i < data.size(); ++i) {
		decrypted[i] ^= key[i % key.size()];
	}
	return decrypted;
}


bool ReadFileContent(const std::string& filepath, std::string& content) {
	std::ifstream file(filepath, std::ios::binary);
	if (!file) {
		std::cerr << "Failed to open file: " << filepath << std::endl;
		return false;
	}

	std::stringstream buffer;
	buffer << file.rdbuf();
	content = buffer.str();

	file.close();
	return true;
}


void WriteDecryptedContent(const std::string& filepath, const std::string& content) {
	std::ofstream file(filepath);
	if (!file) {
		std::cerr << "Failed to write to file: " << filepath << std::endl;
		return;
	}

	file << content;
	file.close();
}



int main(int argc, char* argv[]) {
	if (argc != 3) {
		std::cerr << "Usage: Decryptor <encrypted_file_path> <output_file_path>" << std::endl;
		return 1;
	}

	std::string encryptedFilePath = argv[1];
	std::string outputFilePath = argv[2];
	std::string key = "MiniKeylogger";

	std::string encryptedContent;
	if (!ReadFileContent(encryptedFilePath, encryptedContent)) {
		return 1;
	}

	std::string decryptedContent = Decrypt(encryptedContent, key);

	WriteDecryptedContent(outputFilePath, decryptedContent);

	std::cout << "Decryption completed." << std::endl;

	return 0;
}
