/* �������ͨ�ýӿ� ���ǰ���������ֺ��Ƿ�ɼ� ���Բ�����Ͻ� �����ٸĶ���
* ������˵�� draw������Ҫÿ�����ʵ��һ��ģ�
*/
#ifndef IIMGUIPANEL_H
#define IIMGUIPANEL_H

#include "imgui.h"

class IImGuiPanel {
public:
    virtual ~IImGuiPanel() = default; // OOP�����õ���

    // ���麯�� ÿ����嶼Ҫʵ��Draw -��̬
    virtual void Draw() = 0;

    bool IsVisible = true;        // ��������Ƿ�ɼ�
    const char* PanelName = "Panel"; // Ĭ������
};

#endif 