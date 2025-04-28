import numpy 
import sys

if(len(sys.argv)<=1):
	zi = 1;
else:	
	zi = sys.argv[1];

if(len(sys.argv)<=2):
	which_cluster = 64;
else:
	which_cluster = sys.argv[2];

if(which_cluster == "9999"):

	f = open("../../result/CPU/plot_extended_data_figure_4a.py","w");
	f.write("import	matplotlib.pyplot as plt\n\n");
	
	f_verilog = open("../../result/CPU/extended_data_figure_4a_original_data.v","r")
	
	BDD_depth = 0;
	BDD_width_each_layer = [];
	
	for i in range(0,1000000):
		line = f_verilog.readline();
		if(line[0:9] == "endmodule"):
			break
		elif(line[0:2] == "//"):
			continue
		elif(line[0:4] == "wire"):
			if(line[10] == "-"):
				pass;
				#print(line[10:12]);
			elif(line[7] == ":"):
				BDD_depth += 1;
				BDD_width_each_layer.append(1+int(line[6:7]));
			elif(line[8] == ":"):
				BDD_depth += 1;
				BDD_width_each_layer.append(1+int(line[6:8]));
			elif(line[9] == ":"):
				BDD_depth += 1;
				BDD_width_each_layer.append(1+int(line[6:9]));
			elif(line[10] == ":"):
				BDD_depth += 1;
				BDD_width_each_layer.append(1+int(line[6:10]));
			elif(line[11] == ":"):
				BDD_depth += 1;
				BDD_width_each_layer.append(1+int(line[6:11]));
			elif(line[12] == ":"):
				BDD_depth += 1;
				BDD_width_each_layer.append(1+int(line[6:12]));
				#print(line[10:12]);
		elif(line[0:6] == "assign"):
			if(line[7:9] == "l_"):
				this_node 	= -1;
				this_node_son 	= -1;
				this_node_son_2 = -1;
				this_node_separate = 0;
				for j0 in range(0,1000):
					if(line[j0] == "&"):
						this_node_separate = 1;
						break
					if(line[j0] == "\n"):
						break
	
				for j0 in range(0,1000):
					if(line[j0] == "["):
						break
					if(line[j0] == "\n"):
						break
				if(line[j0] == "["):
					this_node_depth = int(line[9:j0]);
				for j1 in range(j0,1000):
					if(line[j1] == "]"):
						break
					if(line[j1] == "\n"):
						break
				if(line[j1] == "]"):
					this_node	= int(line[j0+1:j1]);	
				for j2 in range(j1,1000):
					if(line[j2:j2+2] == "l_"):
						break
					if(line[j2] == "\n"):
						break
				if(line[j2:j2+2] == "l_"):
					for j3 in range(j2,1000):
						if(line[j3] == "["):
							break
						if(line[j3] == "\n"):
							break
					if(line[j3] == "["):
						for j4 in range(j3,1000):
							if(line[j4] == "]"):
								break
							if(line[j4] == "\n"):
								break
						if(line[j4] == "]"):
							this_node_son	= int(line[j3+1:j4])	
					for j5 in range(j4,1000):
						if(line[j5:j5+2] == "l_"):
							break
						if(line[j5] == "\n"):
							break
					if(line[j5:j5+2] == "l_"):
						for j6 in range(j5,1000):
							if(line[j6] == "["):
								break
							if(line[j6] == "\n"):
								break
						if(line[j6] == "["):
							for j7 in range(j6,1000):
								if(line[j7] == "]"):
									break
								if(line[j7] == "\n"):
									break
							if(line[j7] == "]"):
								this_node_son_2	= int(line[j6+1:j7])	
	
				#print(this_node_depth,this_node,this_node_son,this_node_son_2)
				if((this_node > -1) and (this_node_son> -1)):
					this_node_position 		= (1+this_node)/(BDD_width_each_layer[this_node_depth]+1)
					this_node_son_position		= (1+this_node_son)/(BDD_width_each_layer[this_node_depth+1]+1)
					if(this_node_separate):
						f.write("plt.plot(["+str(this_node_position)+","+str(this_node_son_position)+"],["+str(0-this_node_depth)+","+str(-1-this_node_depth)+"],linewidth=0.3,color='black',marker='.',markersize='0.3',mec='black');\n");
						#f.write("plt.plot(["+str(this_node)+","+str(this_node_son)+"],["+str(0-this_node_depth)+","+str(-1-this_node_depth)+"],linewidth=0.3,color='black',marker='.',markersize='0.3',mec='black');\n");
					else:
						f.write("plt.plot(["+str(this_node_position)+","+str(this_node_son_position)+"],["+str(0-this_node_depth)+","+str(-1-this_node_depth)+"],linewidth=0.1,color='black');\n");
						#f.write("plt.plot(["+str(this_node)+","+str(this_node_son)+"],["+str(0-this_node_depth)+","+str(-1-this_node_depth)+"],linewidth=0.1,color='black');\n");
					if(this_node_son_2>-1):
						this_node_son_2_position	= (1+this_node_son_2)/(BDD_width_each_layer[this_node_depth+1]+1)
						f.write("plt.plot(["+str(this_node_position)+","+str(this_node_son_2_position)+"],["+str(0-this_node_depth)+","+str(-1-this_node_depth)+"],linewidth=0.3,color='black',marker='.',markersize='0.3',mec='black');\n");
						#f.write("plt.plot(["+str(this_node)+","+str(this_node_son_2)+"],["+str(0-this_node_depth)+","+str(-1-this_node_depth)+"],linewidth=0.3,color='black',marker='.',markersize='0.3',mec='black');\n");
					
		else:
			pass
			#print(line);
	print("BDD depth = ",BDD_depth)
	print("BDD_width_each_layer = ",BDD_width_each_layer)
	print("finish generate"+" result/CPU/plot_extended_data_figure_4a.py")
	
	
	f.write("plt.xticks([]);\n");
	f.write("plt.yticks([]);\n");
	f.write("plt.axis('off');\n");
	f.write("plt.savefig('extended_data_figure_4a.png');\n");
	
	f.close();

else:

	f = open("../../result/CPU/cluster_"+str(which_cluster)+"/plot/plot_BDD_"+str(zi)+".py","w");
	f.write("import	matplotlib.pyplot as plt\n\n");
	
	f_verilog = open("../../result/CPU/cluster_"+str(which_cluster)+"/rtl/module_output_bit_"+str(zi)+".v","r")
	
	BDD_depth = 0;
	BDD_width_each_layer = [];
	
	for i in range(0,1000000):
		line = f_verilog.readline();
		if(line[0:9] == "endmodule"):
			break
		elif(line[0:2] == "//"):
			continue
		elif(line[0:4] == "wire"):
			if(line[10] == "-"):
				pass;
				#print(line[10:12]);
			elif(line[7] == ":"):
				BDD_depth += 1;
				BDD_width_each_layer.append(1+int(line[6:7]));
			elif(line[8] == ":"):
				BDD_depth += 1;
				BDD_width_each_layer.append(1+int(line[6:8]));
			elif(line[9] == ":"):
				BDD_depth += 1;
				BDD_width_each_layer.append(1+int(line[6:9]));
			elif(line[10] == ":"):
				BDD_depth += 1;
				BDD_width_each_layer.append(1+int(line[6:10]));
			elif(line[11] == ":"):
				BDD_depth += 1;
				BDD_width_each_layer.append(1+int(line[6:11]));
			elif(line[12] == ":"):
				BDD_depth += 1;
				BDD_width_each_layer.append(1+int(line[6:12]));
				#print(line[10:12]);
		elif(line[0:6] == "assign"):
			if(line[7:9] == "l_"):
				this_node 	= -1;
				this_node_son 	= -1;
				this_node_son_2 = -1;
				this_node_separate = 0;
				for j0 in range(0,1000):
					if(line[j0] == "&"):
						this_node_separate = 1;
						break
					if(line[j0] == "\n"):
						break
	
				for j0 in range(0,1000):
					if(line[j0] == "["):
						break
					if(line[j0] == "\n"):
						break
				if(line[j0] == "["):
					this_node_depth = int(line[9:j0]);
				for j1 in range(j0,1000):
					if(line[j1] == "]"):
						break
					if(line[j1] == "\n"):
						break
				if(line[j1] == "]"):
					this_node	= int(line[j0+1:j1]);	
				for j2 in range(j1,1000):
					if(line[j2:j2+2] == "l_"):
						break
					if(line[j2] == "\n"):
						break
				if(line[j2:j2+2] == "l_"):
					for j3 in range(j2,1000):
						if(line[j3] == "["):
							break
						if(line[j3] == "\n"):
							break
					if(line[j3] == "["):
						for j4 in range(j3,1000):
							if(line[j4] == "]"):
								break
							if(line[j4] == "\n"):
								break
						if(line[j4] == "]"):
							this_node_son	= int(line[j3+1:j4])	
					for j5 in range(j4,1000):
						if(line[j5:j5+2] == "l_"):
							break
						if(line[j5] == "\n"):
							break
					if(line[j5:j5+2] == "l_"):
						for j6 in range(j5,1000):
							if(line[j6] == "["):
								break
							if(line[j6] == "\n"):
								break
						if(line[j6] == "["):
							for j7 in range(j6,1000):
								if(line[j7] == "]"):
									break
								if(line[j7] == "\n"):
									break
							if(line[j7] == "]"):
								this_node_son_2	= int(line[j6+1:j7])	
	
				#print(this_node_depth,this_node,this_node_son,this_node_son_2)
				if((this_node > -1) and (this_node_son> -1)):
					this_node_position 		= (1+this_node)/(BDD_width_each_layer[this_node_depth]+1)
					this_node_son_position		= (1+this_node_son)/(BDD_width_each_layer[this_node_depth+1]+1)
					if(this_node_separate):
						f.write("plt.plot(["+str(this_node_position)+","+str(this_node_son_position)+"],["+str(0-this_node_depth)+","+str(-1-this_node_depth)+"],linewidth=0.3,color='black',marker='.',markersize='0.3',mec='black');\n");
						#f.write("plt.plot(["+str(this_node)+","+str(this_node_son)+"],["+str(0-this_node_depth)+","+str(-1-this_node_depth)+"],linewidth=0.3,color='black',marker='.',markersize='0.3',mec='black');\n");
					else:
						f.write("plt.plot(["+str(this_node_position)+","+str(this_node_son_position)+"],["+str(0-this_node_depth)+","+str(-1-this_node_depth)+"],linewidth=0.1,color='black');\n");
						#f.write("plt.plot(["+str(this_node)+","+str(this_node_son)+"],["+str(0-this_node_depth)+","+str(-1-this_node_depth)+"],linewidth=0.1,color='black');\n");
					if(this_node_son_2>-1):
						this_node_son_2_position	= (1+this_node_son_2)/(BDD_width_each_layer[this_node_depth+1]+1)
						f.write("plt.plot(["+str(this_node_position)+","+str(this_node_son_2_position)+"],["+str(0-this_node_depth)+","+str(-1-this_node_depth)+"],linewidth=0.3,color='black',marker='.',markersize='0.3',mec='black');\n");
						#f.write("plt.plot(["+str(this_node)+","+str(this_node_son_2)+"],["+str(0-this_node_depth)+","+str(-1-this_node_depth)+"],linewidth=0.3,color='black',marker='.',markersize='0.3',mec='black');\n");
					
		else:
			pass
			#print(line);
	print("BDD depth = ",BDD_depth)
	print("BDD_width_each_layer = ",BDD_width_each_layer)
	print("finish generate"+" result/CPU/cluster_"+which_cluster+"/plot/plot_BDD_"+str(zi)+".py")
	
	
	f.write("plt.xticks([]);\n");
	f.write("plt.yticks([]);\n");
	f.write("plt.axis('off');\n");
	f.write("plt.savefig('plot_BDD_"+str(zi)+".png');\n");

	f.close();
