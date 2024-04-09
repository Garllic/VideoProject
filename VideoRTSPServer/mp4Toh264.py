from moviepy.editor import VideoFileClip

input_file = '2.mp4'  # 输入的 MP4 文件路径
output_file = '2.h264'  # 输出的 H.264 文件路径

try:
    # 加载视频文件
    video = VideoFileClip(input_file)
    
    # 使用 H.264 编码保存视频
    video.write_videofile(output_file, codec='libx264')
    
    print(f"转换完成：{output_file}")
except Exception as e:
    print(f"转换失败：{e}")
