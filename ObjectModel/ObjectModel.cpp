void ObjectModel::draw(unsigned int shaderProgram, const glm::mat4& modelMatrix) {
    for (const auto& submesh : submeshes_) {
        // 绑定纹理
        if (submesh.textureID != 0) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, submesh.textureID);
        }
        
        // 绑定VAO
        glBindVertexArray(submesh.VAO);
        
        // 绘制
        glDrawElements(GL_TRIANGLES, submesh.indexCount, GL_UNSIGNED_INT, 0);
        
        // 解绑VAO
        glBindVertexArray(0);
        
        // 解绑纹理
        if (submesh.textureID != 0) {
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }
} 