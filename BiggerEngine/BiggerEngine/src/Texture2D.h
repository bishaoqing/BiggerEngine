#ifndef _H_TEXTURE2D_H_
#define _H_TEXTURE2D_H_

class Texture2D
{
public:
	Texture2D(char const * filename, const int mode = GL_RGB) {
		glGenTextures(1, &m_iTextureID);
		glBindTexture(GL_TEXTURE_2D, m_iTextureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		unsigned char* data = stbi_load(filename, &m_iWidth, &m_iHeight, &m_iChannels, 0);
		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, mode, m_iWidth, m_iHeight, 0, mode, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			std::cout << "Load image error:"<< filename << std::endl;
		}
		stbi_image_free(data);
	}

	void Use(int textureSlot = GL_TEXTURE0) {
		glActiveTexture(textureSlot);
		glBindTexture(GL_TEXTURE_2D, m_iTextureID);
	}
protected:
private:
	unsigned int m_iTextureID;
	int m_iWidth, m_iHeight, m_iChannels;
};


#endif
