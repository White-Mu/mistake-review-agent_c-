#include "nodes/node12_output.h"
#include "utils.h"
#include <iostream>

namespace node12
{
    std::string run(const std::string& final_content)
    {
        utils::print_separator();
        std::cout << "📋 最终错题整理报告\n";
        utils::print_separator();

        // 控制台输出用格式化后的纯文本，看着干净
        std::string display_content = utils::format_for_console(final_content);
        std::cout << "\n" << display_content << "\n";

        utils::print_separator();

        // 保存到文件还是用原始Markdown格式，方便用编辑器打开
        utils::save_to_mistake_book(final_content);
        std::cout << "✅ 已自动保存到总错题本（Markdown格式，用记事本/浏览器打开都好看）\n\n";

        // 1. 询问是否导出单份报告
        std::string choice;
        std::cout << "❓ 是否需要导出单份报告文件？(y/n，默认n)：";
        std::getline(std::cin, choice);
        choice = utils::trim(choice);
        if (choice.empty()) choice = "n";

        if (choice == "y" || choice == "Y")
        {
            std::cout << "📂 请输入导出路径（直接回车默认保存在当前目录）：";
            std::cout << "\n   示例：D:/错题/  或者  C:/Users/XXX/Desktop/\n";
            std::string path;
            std::getline(std::cin, path);
            path = utils::trim(path);

            if (path.empty())
            {
                path = "./";
            }
            else
            {
                if (path.back() != '/' && path.back() != '\\')
                {
                    path += "/";
                }
            }

            std::string filename;
            bool success = utils::export_report_to_path(final_content, path, filename);
            if (success)
            {
                std::cout << "✅ 报告导出成功！文件路径：" << filename << "\n";
            }
            else
            {
                std::cout << "❌ 导出失败！请检查路径是否存在、是否有写入权限\n";
            }
        }
        else
        {
            std::cout << "ℹ️  已跳过导出\n";
        }

        std::cout << "\n";

        // 2. 询问是否查看最近错题本
        std::cout << "❓ 是否查看最近的错题记录？(y/n，默认n)：";
        std::getline(std::cin, choice);
        choice = utils::trim(choice);
        if (choice.empty()) choice = "n";

        if (choice == "y" || choice == "Y")
        {
            std::cout << "\n" << utils::get_recent_mistakes(3) << "\n";

            std::cout << "❓ 是否用记事本打开完整错题本？(y/n，默认n)：";
            std::getline(std::cin, choice);
            choice = utils::trim(choice);
            if (choice.empty()) choice = "n";
            if (choice == "y" || choice == "Y")
            {
                std::cout << "📖 正在打开错题本...\n";
                utils::open_mistake_book();
            }
            else
            {
                std::cout << "ℹ️  已跳过打开\n";
            }
        }
        else
        {
            std::cout << "ℹ️  已跳过查看\n";
        }

        std::cout << "\n🎉 全部处理完成！\n";
        return final_content;
    }
}