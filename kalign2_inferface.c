/*
	kalign2_interface.c 
	
	Released under GPL - see the 'COPYING' file   
	
	Copyright (C) 2006 Timo Lassmann <timolassmann@gmail.com>
	
	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
    
	Please send bug reports, comments etc. to:
	timolassmann@gmail.com
*/

#include <string.h>

#include <getopt.h>

//#include <unistd.h>
#include "kalign2.h"
#include "kalign2_input.h"

void parameter_message(struct parameters* param)
{
        if(param->infile[0]){
                if(param->dna){
                        fprintf(stderr,"Aligning %d RNA/DNA sequences from file %s with these parameters:\n",numseq,param->infile[0]);
                }else{
                        fprintf(stderr,"Aligning %d protein sequences from file %s with these parameters:\n",numseq,param->infile[0]);
                }
        }else{
                if(param->dna){
                        fprintf(stderr,"Aligning %d RNA/DNA sequences with these parameters:\n",numseq);
                }else{
                        fprintf(stderr,"Aligning %d protein sequences with these parameters:\n",numseq);
                }
                
        }
       // fprintf(stderr,"	Distance estimation method:   %s\n",param->distance);
	//fprintf(stderr,"	Tree building method:         %s\n",param->tree);
	//fprintf(stderr,"	Ntree:                        %d\n",param->ntree);
        //fprintf(stderr,"        %0.8f	gap open penalty\n",(float)gpo/5);
	 fprintf(stderr,"        %0.8f	gap open penalty\n",(float)gpo);
        //fprintf(stderr,"        %0.8f	gap extension\n",(float)gpe/10);
	fprintf(stderr,"        %0.8f	gap extension\n",(float)gpe);
        //fprintf(stderr,"        %0.8f	terminal gap penalty\n",(float)tgpe/10);
	fprintf(stderr,"        %0.8f	terminal gap penalty\n",(float)tgpe);
        //fprintf(stderr,"        %0.8f	bonus\n",param->secret/10);
        fprintf(stderr,"        %0.8f	bonus\n",param->secret);
        /*fprintf(stderr,"        %0.2f	zlevel\n",param->zlevel);
	fprintf(stderr,"        %0.2f	igw\n",param->internal_gap_weight);
	fprintf(stderr,"        %d	sw\n",param->smooth_window);
        fprintf(stderr,"        %0.2f	smo\n", param->gap_inc);*/
     
	if(param->outfile){
		fprintf(stderr,"Alignment will be written to file:'%s'.\n\n",param->outfile);
	}else{
		fprintf(stderr,"Alignment will be written to stdout.\n\n");
	}
}

// Additions for Du Novo

int is_printable_str(char *str) {
  if ((long int)str > -1000 && (long int)str < 1000) {
    return 0;
  }
  int i = 0;
  while (str[i] != 0) {
    if (str[i] < 32 || str[i] > 126) {
      return 0;
    }
    i++;
  }
  return 1;
}

void print_suspicious_str(char *name, char *str) {
  printf("%s: ", name);
  if (str == 0) {
    printf("(null)\n");
  } else if ((long int)str > -1000 && (long int)str < 1000) {
    printf("%ld\n", (long int)str);
  } else if (is_printable_str(str)) {
    printf("%s\n", str);
  } else {
    printf("[non-str]\n");
  }
}

void print_param(struct parameters *param) {
  int i;
  printf("Parameters:\n");
  char *name = malloc(sizeof(char) * 20);
  for (i = 0; i < 4; i++) {
    sprintf(name, "param->infile[%d]", i);
    print_suspicious_str(name, param->infile[i]);
  }
  print_suspicious_str("param->input", param->input);
  print_suspicious_str("param->outfile", param->outfile);
  print_suspicious_str("param->format", param->format);
  print_suspicious_str("param->feature_type", param->feature_type);
  print_suspicious_str("param->alignment_type", param->alignment_type);
  print_suspicious_str("param->feature_mode", param->feature_mode);
  print_suspicious_str("param->distance", param->distance);
  print_suspicious_str("param->tree", param->tree);
  print_suspicious_str("param->sort", param->sort);
  print_suspicious_str("param->sub_matrix", param->sub_matrix);
  print_suspicious_str("param->print_tree", param->print_tree);
  print_suspicious_str("param->print_svg_tree", param->print_svg_tree);
  printf("param->gpo: %f\n", param->gpo);
  printf("param->gpe: %f\n", param->gpe);
  printf("param->tgpe: %f\n", param->tgpe);
  printf("param->secret: %f\n", param->secret);
  printf("param->zlevel: %f\n", param->zlevel);
  printf("param->same_feature_score: %f\n", param->same_feature_score);
  printf("param->diff_feature_score: %f\n", param->diff_feature_score);
  printf("param->reformat: %d\n", param->reformat);
  printf("param->id: %d\n", param->id);
  printf("param->aa: %d\n", param->aa);
  printf("param->alter_gaps: %d\n", param->alter_gaps);
  printf("param->ntree: %d\n", param->ntree);
  printf("param->help_flag: %d\n", param->help_flag);
  printf("param->quiet: %d\n", param->quiet);
  printf("param->dna: %d\n", param->dna);
  printf("param->alter_range: %f\n", param->alter_range);
  printf("param->alter_weight: %d\n", param->alter_weight);
  printf("param->internal_gap_weight: %f\n", param->internal_gap_weight);
  printf("param->smooth_window: %d\n", param->smooth_window);
  printf("param->gap_inc: %f\n", param->gap_inc);
  printf("End of parameters.\n\n");
}

struct parameters *make_param(struct parameters *param, char *infile, char *outfile) {
  param->infile = malloc(sizeof(char*)*3);
  param->infile[0] = 0;
  param->infile[1] = infile;
  param->infile[2] = 0;
  param->input = 0;
  param->outfile = outfile;
  param->format = 0;
  param->feature_type = 0;
  param->alignment_type = "default";
  param->distance = "wu";
  param->sort = 0;
  param->sub_matrix = 0;
  param->print_tree = 0;
  param->print_svg_tree = 0;
  param->tree = "upgma";
  param->gpo = -1.0; // default: -1.0 (Gap open penalty)
  param->gpe = -1.0;
  param->tgpe = -0.95; // default: -1.0 (Terminal gap extension penalty)
  param->secret = -1.0;
  param->zlevel = 58.8;
  param->same_feature_score = 75;
  param->diff_feature_score = -5;
  param->reformat = 0;
  param->id = -1;
  param->aa = 0;
  param->alter_gaps = 0;
  param->ntree = 2;
  param->help_flag = 0;
  param->quiet = 1;
  param->dna = -1;
  param->alter_range = 0.5;
  param->alter_weight = 100;
  param->internal_gap_weight = 0;
  param->smooth_window = 1;
  param->gap_inc = 0.0;
  return param;
}

// End Du Novo

struct parameters* interface(struct parameters* param,int argc,char **argv)
{
	int i,c;

	param->gpo = -1.0;
	param->gpe = -1.0;
	param->tgpe = -1.0;
	param->secret = -1.0;
	param->zlevel = 58.8;
	param->sub_matrix = 0;
	param->aa = 0;
	
	param->infile = malloc(sizeof(char*)*3);
	param->infile[0] = 0;
	param->infile[1] = 0;
	param->infile[2] = 0;
	param->input = 0;
	param->outfile = 0;
	param->format = 0;
	param->help_flag = 0;
	param->quiet = 0;
	param->id = -1;
	param->distance = "wu";
	param->reformat = 0;
	param->sort = 0;
	
	param->print_svg_tree = 0;
	
	param->dna = -1;
	 
	param->feature_type = 0;
	param->alignment_type = "default";
	param->tree = "upgma";
	param->ntree = 2;
	param->print_tree = 0;
	param->alter_gaps = 0;
	param->alter_range = 0.5;
	param->alter_weight = 100;
	
        param->internal_gap_weight = 0;
	param->smooth_window = 1;
        param->gap_inc = 0.0;
	param->same_feature_score = 75;
	param->diff_feature_score = -5;
        
/*
        static char  license[] = "\n\
Kalign version 2.04, Copyright (C) 2004, 2005, 2006 Timo Lassmann\n\n\
        Kalign is free software. You can redistribute it and/or modify\n\
        it under the terms of the GNU General Public License as\n\
        published by the Free Software Foundation.\n\n";
*/
        

	while (1){
        	static struct option long_options[] ={
		{"gapopen",  required_argument, 0,'s'},
		{"gpo",  required_argument, 0, 's'},
		{"gapextension",  required_argument, 0, 'e'},
		{"gpe",  required_argument, 0, 'e'},
                {"secret",  required_argument, 0, 'm'},
                {"bonus",  required_argument, 0, 'm'},
		{"terminalgapextension",  required_argument, 0, 't'},
                {"tgpe",  required_argument, 0, 't'},
                {"zcutoff",  required_argument, 0, 0},
                {"distance",  required_argument, 0, 'd'},
                {"ntree",  required_argument, 0, 0},
                {"tree",  required_argument, 0, 0},
                {"format",  required_argument, 0, 'f'},
                {"reformat",  0, 0, 'r'},
                {"sort",required_argument,0,'c'},
                {"feature",  required_argument, 0, 0},
		{"type",  required_argument, 0, 0},
		{"alter_gaps",  required_argument, 0, 0},
		{"altergaps",  required_argument, 0, 0},
		{"alter_range",  required_argument, 0, 0},
		{"alter_weight",  required_argument, 0, 0},
		{"internal_gap_weight",  required_argument, 0, 0},
		{"smooth_window",  required_argument, 0, 0},
		{"gap_inc",  required_argument, 0, 'a'},
		{"matrix",  required_argument, 0, 0},
		{"mmbonus",  required_argument, 0, 0},
		{"nuc",  0, 0, 0},
		{"dna",  0, 0, 0},
		{"rna",  0, 0, 0},
		{"protein",  0, 0, 0},
		{"profile", 0, 0, 0},
		{"prof", 0, 0, 0},
		{"id", required_argument, 0, 0},
		{"printtree", required_argument, 0, 0},
		{"svgtree", required_argument, 0, 0},
		{"svg_tree", required_argument, 0, 0},
		{"pairwise", 0, 0, 0},
		{"same_feature_score", required_argument, 0, 0},
		{"diff_feature_score", required_argument, 0, 0},
		
                {"input",  required_argument, 0, 'i'},
                {"infile",  required_argument, 0, 'i'},
                {"in",  required_argument, 0, 'i'},
                {"output",  required_argument, 0, 'o'},
                {"outfile",  required_argument, 0, 'o'},
                {"out",  required_argument, 0, 'o'},
                {"help",   no_argument,0,'h'},
                {"quiet",  0, 0, 'q'},
                

                {0, 0, 0, 0}
                };
                
                int option_index = 0;
                c = getopt_long_only (argc, argv,"i:o:hqs:e:m:t:z:c:f:d:b:a:r",long_options, &option_index);
                //c = getopt (argc, argv, "hi:o:");
                /* Detect the end of the options. */
                if (c == -1){
                        break;
                }
                switch (c){
			case 0:
				if (long_options[option_index].flag != 0){
					break;
				}
				switch (option_index){
					case 0:
					case 1:
						fprintf(stderr,"GAGAGA\n");
						param->gpo = atof(optarg);
						break;
					case 2:
					case 3:
						param->gpe = atof(optarg);
						break;
					case 4:
					case 5:
						param->secret = atof(optarg);
						break;
					case 6:
					case 7:
						param->tgpe = atof(optarg);
						break;
					case 8:
						param->zlevel = atof(optarg);
						break;
					case 9:
						param->distance = optarg;
						break;	
					case 10:
						param->ntree = atoi(optarg);
						break;
					case 11:
						param->tree = optarg;
						break;
					case 12:
						param->format = optarg;
						break;
					case 13:
						param->reformat = 1;
						break;
					case 14:
						param->sort = optarg;
						break;
					case 15:
						param->feature_type = optarg;
						break;
					case 16:
						param->alignment_type = optarg;
						break;
					case 17:
					case 18:
						param->alter_gaps = atoi(optarg);
						break;	
					case 19:
						param->alter_range = atof(optarg);
						break;	
					case 20:
						param->alter_weight = atoi(optarg);
						break;	
					case 21:
						param->internal_gap_weight = atof(optarg);
						break;	
					case 22:
						param->smooth_window = atoi(optarg);
						break;	
					case 23:
						param->gap_inc = atof(optarg);
						break;	
					case 24:
						param->sub_matrix = optarg;
						break;	
					case 25:
						param->aa = atoi(optarg);
						break;	
					case 26:
					case 27:
					case 28:
						param->dna = 1;
						break;
					case 29:
						param->dna = 0;
						break;
					case 30:
					case 31:
						param->alignment_type = "profile";
						break;	
					case 32:
						param->id = atoi(optarg);
						break;
					case 33:
						param->print_tree = optarg;
						break;
					case 34:
					case 35:
						param->print_svg_tree = optarg;
						break;
					case 36:
						param->alignment_type = "pairwise";
						break;
					case 37:
						param->same_feature_score = atof(optarg);//"pairwise";
						break;
					case 38:
						param->diff_feature_score = atof(optarg);//lignment_type = "pairwise";
						break;
					
						
					default:
						break;
				}
				//printf ("option%d %s",option_index,long_options[option_index].name);
				//if (optarg){
				//	printf (" with arg %s\n", optarg);
				//}
				break;
			 case 's':
			 	param->gpo = atof(optarg);
                        	//param->help_flag = 1;
				break;
			case 'e':
				param->gpe = atof(optarg);
				break;
			case 'm':
				param->secret = atof(optarg);
				break;
			case 't':	
				param->tgpe = atof(optarg);
				break;
			case 'z':	
				param->zlevel = atof(optarg);
				break;
				
			case 'c':
				param->sort = optarg;
				break;
			case 'f':
				param->format = optarg;
				break;
			case 'r':
				param->reformat = 1;
				break;
				
			case 'd':
				param->distance = optarg;
				break;	
			case 'b':
				param->tree = optarg;
				break;	
				
				
				
                        case 'h':
                        	param->help_flag = 1;
				break;
                        case 'i':
                                param->infile[1] = optarg;
                                break;
                        case 'o':
                                param->outfile = optarg;
                                break;
			case 'a':
			 	param->gap_inc = atof(optarg);
				break;
                        case 'q':
                                param->quiet = 1;
                                break;
                        case '?':
                                free_param(param);
                                exit(1);
                        break;
                                default:
                                abort ();
                }
        }
      


        if (optind < argc){
                c = 0;
                
                
                if(param->infile[1]){
                	i = 3;
                }else{
                	i = 2;
                }
                param->infile = realloc(param->infile,(sizeof(char*) * (argc - optind+i)));
                for (c = i-1 ; c < (argc - optind+i);c++){
                	param->infile[c] = 0;
                }
                c = i-1;
                while (optind < argc){
                        /*switch(c){
                                case 0:
                                        param->infile[c] =  argv[optind++];
                                        break;
                                case 1:
                                        param->outfile =  argv[optind++];
                                        break;
                                default:
                                        fprintf(stderr,"Unrecognised junk:%s\n",argv[optind++]);
                                        break;
                        }*/
                        
                        param->infile[c] =  argv[optind++];
                        c++;
		}
        }
	//exit(0);

	if (byg_start("pairwise",param->alignment_type) != -1){
		param->ntree = 1;
		if(param->tgpe == -1.0){
			param->tgpe =  0.0f;
		}
	}
	if(param->gap_inc < 0.0){
		fprintf(stderr,"%s\n", usage);
		fprintf(stderr,"Invalid parameter setting: gap_inc needs to be > 0 \n");
		exit(1);
	}
	
	// if(param->quiet){
	// 	fclose(stderr);
	// }
	// fprintf(stderr,"%s", license);
	// if (param->help_flag){
	// 	fprintf(stderr,"%s\n", usage);
	// 	exit(1);
	// }
	//exit(0);
	
        
        /*if (!param->infile[1]){
                fprintf(stderr,"%s\n", usage);
                exit(1);
        }*/
        return param;
}
