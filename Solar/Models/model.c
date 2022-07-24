#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Maths/vec3.h"
#include "Maths/vec2.h"
#include "Solar/Models/model.h"

float *load_obj(const char *path, unsigned int *len){
    Vec3 *vertices = NULL;
    Vec3 *normals = NULL;
    Vec2 *textcoords = NULL;
    float *out = NULL;
    
    unsigned int vertcount = 0;
    unsigned int normcount = 0;
    unsigned int textcount = 0;
    unsigned int outlen = 0;
    
    char symbol[2];

    FILE *file = fopen(path, "r");
    char *line = NULL;
    unsigned int n = 0;
    ssize_t read;
    while((read = getline(&line, &n, file)) != -1){
        if(line[0] == 'v' && line[1] == 'n'){
            //normals for lighting
            float vn1, vn2, vn3;
            sscanf(line, "%s %f %f %f\n", symbol, &vn1, &vn2, &vn3);
            normcount++;
            normals = realloc(normals, normcount * sizeof(Vec3));
            normals[normcount - 1].x = vn1;
            normals[normcount - 1].y = vn2;
            normals[normcount - 1].z = vn3;

        }else if(line[0] == 'v' && line[1] == 't'){
            //texture coordinates
            float vt1, vt2;
            sscanf(line, "%s %f %f\n", symbol, &vt1, &vt2);
            textcount++;
            textcoords = realloc(textcoords, textcount * sizeof(Vec2));
            textcoords[textcount - 1].x = vt1;
            textcoords[textcount - 1].y = vt2;

        }else if(line[0] == 'v'){
            //vertices for model 
            float v1, v2, v3;
            sscanf(line, "%s %f %f %f\n", symbol, &v1, &v2, &v3);
            vertcount++;
            vertices = realloc(vertices, vertcount * sizeof(Vec3));
            vertices[vertcount - 1].x = v1;
            vertices[vertcount - 1].y = v2;
            vertices[vertcount - 1].z = v3;
        }

        if(line[0] == 'f'){
            int v1, v2, v3;
            int vn1, vn2, vn3;
            int vt1, vt2, vt3;
            sscanf(line, "%c %d/%d/%d %d/%d/%d %d/%d/%d\n", &symbol,
                &v1, &v2, &v3, &vt1, &vt2, &vt3, &vn1, &vn2, &vn3);

            outlen += 24;
            out = realloc(out, outlen * sizeof(float));
            memcpy(&out[outlen - 24], &vertices[v1], 3 * sizeof(float));
            memcpy(&out[outlen - 24 + 3], &vertices[v2], 3 * sizeof(float));
            memcpy(&out[outlen - 24 + 6], &vertices[v3], 3 * sizeof(float));

            memcpy(&out[outlen - 24 + 8], &vertices[vt1], 2 * sizeof(float));
            memcpy(&out[outlen - 24 + 10], &vertices[vt2], 2 * sizeof(float));
            memcpy(&out[outlen - 24 + 12], &vertices[vt3], 2 * sizeof(float));

            memcpy(&out[outlen - 24 + 15], &vertices[vn1], 3 * sizeof(float));
            memcpy(&out[outlen - 24 + 18], &vertices[vn2], 3 * sizeof(float));
            memcpy(&out[outlen - 24 + 21], &vertices[vn3], 3 * sizeof(float));
        }
    }

    *len = outlen;
    close(file);
    free(line);
    free(vertices);
    free(textcoords);
    free(normals);
    return out;
}

//int main(){
//    unsigned int len;
//    float *test = load_obj("sphere.obj", &len);
//    free(test);
//    return 0;
//}
