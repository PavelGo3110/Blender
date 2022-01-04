/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * The Original Code is Copyright (C) 2005 Blender Foundation.
 * All rights reserved.
 */

/** \file
 * \ingroup shdnodes
 */

#include "node_shader_util.h"

namespace blender::nodes::node_shader_invert_cc {

/* **************** INVERT ******************** */
static bNodeSocketTemplate sh_node_invert_in[] = {
    {SOCK_FLOAT, N_("Fac"), 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, PROP_FACTOR},
    {SOCK_RGBA, N_("Color"), 0.0f, 0.0f, 0.0f, 1.0f},
    {-1, ""}};

static bNodeSocketTemplate sh_node_invert_out[] = {{SOCK_RGBA, N_("Color")}, {-1, ""}};

static void node_shader_exec_invert(void *UNUSED(data),
                                    int UNUSED(thread),
                                    bNode *UNUSED(node),
                                    bNodeExecData *UNUSED(execdata),
                                    bNodeStack **in,
                                    bNodeStack **out)
{
  float col[3], icol[3], fac;

  nodestack_get_vec(&fac, SOCK_FLOAT, in[0]);
  nodestack_get_vec(col, SOCK_VECTOR, in[1]);

  icol[0] = 1.0f - col[0];
  icol[1] = 1.0f - col[1];
  icol[2] = 1.0f - col[2];

  /* if fac, blend result against original input */
  if (fac < 1.0f) {
    interp_v3_v3v3(out[0]->vec, col, icol, fac);
  }
  else {
    copy_v3_v3(out[0]->vec, icol);
  }
}

static int gpu_shader_invert(GPUMaterial *mat,
                             bNode *node,
                             bNodeExecData *UNUSED(execdata),
                             GPUNodeStack *in,
                             GPUNodeStack *out)
{
  return GPU_stack_link(mat, node, "invert", in, out);
}

}  // namespace blender::nodes::node_shader_invert_cc

void register_node_type_sh_invert()
{
  namespace file_ns = blender::nodes::node_shader_invert_cc;

  static bNodeType ntype;

  sh_node_type_base(&ntype, SH_NODE_INVERT, "Invert", NODE_CLASS_OP_COLOR);
  node_type_socket_templates(&ntype, file_ns::sh_node_invert_in, file_ns::sh_node_invert_out);
  node_type_exec(&ntype, nullptr, nullptr, file_ns::node_shader_exec_invert);
  node_type_gpu(&ntype, file_ns::gpu_shader_invert);

  nodeRegisterType(&ntype);
}