import os
import chardet # 这是一个强大的编码检测库

# --- 配置区 ---
# 你想要转换的文件夹路径
TARGET_FOLDER = '.\\dsc-code' 
# 你想要转换的文件扩展名
FILE_EXTENSIONS = ('.c', '.h') 
# 原始编码（如果 chardet 检测失败，会使用这个作为备用）
FROM_ENCODING = 'gbk' 
# --- 配置区结束 ---

def convert_files_to_utf8(root_dir):
    """
    递归地将指定文件夹内特定扩展名的文件从原始编码转换为 UTF-8。
    """
    print(f"开始扫描文件夹: {os.path.abspath(root_dir)}")
    converted_count = 0
    
    for subdir, _, files in os.walk(root_dir):
        for filename in files:
            if filename.lower().endswith(FILE_EXTENSIONS):
                file_path = os.path.join(subdir, filename)
                
                try:
                    # 以二进制模式读取文件内容
                    with open(file_path, 'rb') as f:
                        content_bytes = f.read()
                    
                    # 如果文件是空的，直接跳过
                    if not content_bytes:
                        continue

                    # 使用 chardet 检测编码
                    detected_result = chardet.detect(content_bytes)
                    encoding = detected_result['encoding']
                    confidence = detected_result['confidence']
                    
                    # 如果检测到的编码不是 UTF-8 且置信度较高，则进行转换
                    # 'ascii' 编码是 UTF-8 的子集，无需转换
                    if encoding and encoding.lower() not in ['utf-8', 'ascii']:
                        print(f"转换文件: {file_path} (检测到编码: {encoding}, 置信度: {confidence:.0%})")
                        
                        # 使用检测到的编码来解码
                        decoded_content = content_bytes.decode(encoding, errors='replace')
                        
                        # 以 UTF-8 编码写回文件
                        with open(file_path, 'w', encoding='utf-8') as f:
                            f.write(decoded_content)
                        converted_count += 1
                    else:
                        print(f"跳过文件: {file_path} (已是 UTF-8 或无法确定)")

                except Exception as e:
                    print(f"处理文件 {file_path} 时发生错误: {e}")

    print(f"\n转换完成！总共转换了 {converted_count} 个文件。")

if __name__ == '__main__':
    if not os.path.isdir(TARGET_FOLDER):
        print(f"错误：目标文件夹 '{TARGET_FOLDER}' 不存在。请检查路径是否正确。")
    else:
        convert_files_to_utf8(TARGET_FOLDER)