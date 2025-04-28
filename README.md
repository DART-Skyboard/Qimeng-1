# Automated CPU Design by Learning from Input-Output Examples

If you find our work useful for your work please cite:
```
@inproceedings{10.24963/ijcai.2024/425,
  author = {Cheng, Shuyao and Jin, Pengwei and Guo, Qi and Du, Zidong and Zhang, Rui and Hu, Xing and Zhao, Yongwei and Hao, Yifan and Guan, Xiangtao and Han, Husheng and Zhao, Zhengyue and Liu, Ximing and Zhang, Xishan and Chu, Yuejie and Mao, Weilong and Chen, Tianshi and Chen, Yunji},
  title = {Automated CPU design by learning from input-output examples},
  year = {2024},
  isbn = {978-1-956792-04-1},
  url = {https://doi.org/10.24963/ijcai.2024/425},
  doi = {10.24963/ijcai.2024/425},
  abstract = {Designing a central processing unit (CPU) requires intensive manual work of talented experts to implement the circuit logic from design specifications. Although considerable progress has been made in electronic design automation (EDA) to relieve human efforts, all existing tools require handcrafted formal program codes (e.g., Verilog, Chisel, or C) as the input. To automate the CPU design without human programming, we are motivated to learn the CPU design from only input-output (IO) examples, which are generated from test cases of design specification. The key challenge is that the learned CPU design should have almost zero tolerance for inaccuracy, which makes well-known approximate algorithms such as neural networks ineffective.We propose a new AI approach to generate the CPU design in the form of a large-scale Boolean function, from only external IO examples instead of formal program code. This approach employs a novel graph structure called Binary Speculative Diagram (BSD) to approximate the CPU-scale Boolean function accurately. We propose an efficient BSD expansion method based on Boolean Distance, a new metric to quantitatively measure the structural similarity between Boolean functions, gradually increasing the design accuracy up to 100\%. Our approach generates an industrial-scale RISC-V CPU design within 5 hours, reducing the design cycle by about 1000\texttimes{} without human involvement. The taped-out chip, Enlightenment-1, the world's first CPU designed by AI, successfully runs the Linux operating system and performs comparably against the human-design Intel 80486SX CPU. Our approach even autonomously discovers human knowledge of the von Neumann architecture.},
  booktitle = {Proceedings of the Thirty-Third International Joint Conference on Artificial Intelligence},
  articleno = {425},
  numpages = {11},
  location = {Jeju, Korea},
  series = {IJCAI '24}
}
```

# Website License
<a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-sa/4.0/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/">Creative Commons Attribution-ShareAlike 4.0 International License</a>.