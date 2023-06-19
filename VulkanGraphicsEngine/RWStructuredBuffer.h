#pragma once

//https://2.bp.blogspot.com/-EpIVpOGFHYI/XuGTbz7skEI/AAAAAAAACdA/7zx2iybU85AqJfUROEHu1NACRXI5aeCUgCLcBGAsYHQ/s1600/20200611.jpg

class RWStructuredBuffer{
    void Init(const VulkanVertexBuffer& vb, bool isUpdatedByCPU);

    void Init(const VulkanIndexBuffer& ib, bool usUpdatedByCPU);



    

    void RegistImageView();

    VkBuffer GetVkBuffer();

private:
    VkBuffer m_buffersOnGPU[2] = { nullptr };
    std::shared_ptr<void> m_buffersOnCPU[2];
    int m_numElement = 0;
    int m_sizeOfElement = 0;
    bool m_isInited = false;



    void RegistImageView(int width, int height);

}



