This file will explain the pre fixes and the values associated

n square 	-- (Prefix: Name , filename of the mesh(.msh))

v -0.5 -0.5	-- (Prefix: Vertex, Model x value of the vertex Model y value of the vertex) // NOTE: it currently draws huge meshes because Model coords(-0.5,-0.5) to (0.5,0.5) are being parsed as NDC coords into opengl API, except for circles which are (-1,-1) to (1,1)

rgb 1 0 0 	-- (Prefix: rgb, red value, green value, blue value) // NOTE: rgb values are only valid in the range (0.01 to 1.00)

t 0		-- (Prefix: t, index for the ordered vertex) // NOTE: GL_TRIANGLES will be initialised as the render mode for glDrawElements()

s 0		-- (Prefix: s, index for the ordered vertex) // NOTE: GL_TRIANGLE_STRIP will be initialised as the render mode for glDrawElements()

f 720		-- (Prefix: f, index for the ordered vertex) // NOTE: GL_TRIANGLE_FAN will be initialised as the render mode for glDrawElements() AND for cicles, it will draw from 720th vertex and back to the first vertex


idx_cnt 4	-- (Prefix: idx_cnt, order of vertices to render), when parsed into glDrawArrays ( it will only take in the first 4 values for drawing the quad using GL_TRIANGLES)

bb_idx_cnt 8	-- (Prefix: bb_idx_cnt, order of vertices to render a bounding box around the mesh) when parsed into glDrawArrays ( it will take in all 8 values, to draw the bounding box using GL_LINES)