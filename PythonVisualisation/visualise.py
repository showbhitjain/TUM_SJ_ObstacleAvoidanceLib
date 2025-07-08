import numpy as np
import pandas as pd
from fontTools.unicodedata import block
# from roboticstoolbox import DHRobot, RevoluteMDH
import json
# import roboticstoolbox as rtb
import math
from spatialmath.base import *
from spatialmath import *
import matplotlib.pyplot as plt
import time
from matplotlib.animation import FuncAnimation
import matplotlib.animation as animation
from mpl_toolkits.mplot3d import Axes3D
from mpl_toolkits.mplot3d.art3d import Poly3DCollection
from itertools import cycle
import matplotlib
from scipy.spatial.transform import Rotation
matplotlib.use('QT5Agg')


def plot_cylinder(ax, v0, v1, radius, face_color='r', face_opacity=0.3, num_points=20):
    # Calculate cylinder length
    height = np.linalg.norm(v1 - v0)

    # Create the base cylinder along the Z-axis
    z = np.linspace(0, height, num_points)
    theta = np.linspace(0, 2 * np.pi, num_points)
    theta, z = np.meshgrid(theta, z)
    x = radius * np.cos(theta)
    y = radius * np.sin(theta)

    # Calculate the direction vector and rotation matrix
    direction = (v1 - v0) / np.linalg.norm(v1 - v0)
    up = np.array([0, 0, 1])

    if np.allclose(direction, up):
        R = np.eye(3)
    else:
        axis = np.cross(up, direction)
        angle = np.arccos(np.dot(up, direction))
        axis = axis / np.linalg.norm(axis)

        K = np.array([
            [0, -axis[2], axis[1]],
            [axis[2], 0, -axis[0]],
            [-axis[1], axis[0], 0]
        ])
        R = np.eye(3) + np.sin(angle) * K + (1 - np.cos(angle)) * np.dot(K, K)

    # Rotate and translate the cylinder points
    points = np.vstack((x.ravel(), y.ravel(), z.ravel()))
    new_points = R @ points
    x = new_points[0, :].reshape(x.shape) + v0[0]
    y = new_points[1, :].reshape(y.shape) + v0[1]
    z = new_points[2, :].reshape(z.shape) + v0[2]

    # Plot the cylinder surface
    plots = ax.plot_surface(x, y, z, color=face_color, alpha=face_opacity, edgecolor='none')
    ax.set_xlabel('X-axis')
    ax.set_ylabel('Y-axis')
    ax.set_zlabel('Z-axis')
    return plots


def plot_cylinder_new(ax, radius_cylinder, cylinder_height, cylinder_center, cylinder_axis,
                      face_color='lightblue', face_opacity=0.7, resolution=50):
    """
    Plot a cylinder on the provided 3D axis.

    Parameters:
      - ax: a matplotlib 3D axis.
      - radius_cylinder: radius of the cylinder.
      - cylinder_height: height of the cylinder.
      - cylinder_center: [x, y, z] coordinates for the center of the cylinder.
      - cylinder_axis: normalized 3D vector (array-like) indicating the direction of the cylinder's axis.
      - face_color: color of the cylinder.
      - face_opacity: transparency (alpha value) of the cylinder.
      - resolution: number of subdivisions for the surface mesh.

    Returns:
      - plots: the surface plot object created by ax.plot_surface.
    """
    # Create parameters for the cylinder.
    theta = np.linspace(0, 2 * np.pi, resolution)
    z_lin = np.linspace(-cylinder_height / 2, cylinder_height / 2, resolution)
    theta_grid, z_grid = np.meshgrid(theta, z_lin)

    # Create two vectors perpendicular to the cylinder_axis.
    def get_orthonormal_basis(axis):
        arbitrary_vec = np.array([0, 0, 1])
        if np.allclose(axis, arbitrary_vec):
            arbitrary_vec = np.array([0, 1, 0])
        basis_vec1 = np.cross(axis, arbitrary_vec)
        basis_vec1 = basis_vec1 / np.linalg.norm(basis_vec1)
        basis_vec2 = np.cross(axis, basis_vec1)
        basis_vec2 = basis_vec2 / np.linalg.norm(basis_vec2)
        return basis_vec1, basis_vec2

    basis_vec1, basis_vec2 = get_orthonormal_basis(np.array(cylinder_axis))

    # Compute the points on the cylinder's surface.
    x_grid = (cylinder_center[0] +
              z_grid * cylinder_axis[0] +
              radius_cylinder * np.cos(theta_grid) * basis_vec1[0] +
              radius_cylinder * np.sin(theta_grid) * basis_vec2[0])
    y_grid = (cylinder_center[1] +
              z_grid * cylinder_axis[1] +
              radius_cylinder * np.cos(theta_grid) * basis_vec1[1] +
              radius_cylinder * np.sin(theta_grid) * basis_vec2[1])
    z_grid_plot = (cylinder_center[2] +
                   z_grid * cylinder_axis[2] +
                   radius_cylinder * np.cos(theta_grid) * basis_vec1[2] +
                   radius_cylinder * np.sin(theta_grid) * basis_vec2[2])

    # Plot the cylinder surface.
    plots = ax.plot_surface(x_grid, y_grid, z_grid_plot, color=face_color,
                            alpha=face_opacity, edgecolor='none')

    # Optionally, you could also plot the central axis if desired.
    # (Here we focus on returning the surface plot as with the sphere.)

    return plots


def plot_box(ax, box_center, box_dimensions, box_orientation, face_color='blue', face_opacity=0.3):
    """
    Plot a box on the provided 3D axis.

    The box is defined by its center, full dimensions along x, y, and z,
    and its orientation given as a quaternion [w, x, y, z].

    Parameters:
      - ax: a matplotlib 3D axis.
      - box_center: [x, y, z] coordinates for the center of the box.
      - box_dimensions: [length_x, length_y, length_z] full lengths in each direction.
      - box_orientation: a 4-element numpy array representing the quaternion [w, x, y, z].
      - face_color: color of the box faces.
      - face_opacity: transparency (alpha value) of the box faces.

    Returns:
      - plots: the Poly3DCollection object representing the box faces.
    """

    # Convert the quaternion to a 3x3 rotation matrix.
    def quaternion_to_rotation_matrix(q):
        w, x, y, z = q
        r00 = 1 - 2 * (y ** 2 + z ** 2)
        r01 = 2 * (x * y - z * w)
        r02 = 2 * (x * z + y * w)
        r10 = 2 * (x * y + z * w)
        r11 = 1 - 2 * (x ** 2 + z ** 2)
        r12 = 2 * (y * z - x * w)
        r20 = 2 * (x * z - y * w)
        r21 = 2 * (y * z + x * w)
        r22 = 1 - 2 * (x ** 2 + y ** 2)
        return np.array([[r00, r01, r02],
                         [r10, r11, r12],
                         [r20, r21, r22]])

    rotation_matrix = quaternion_to_rotation_matrix(box_orientation)

    # Get the local coordinates for the 8 corners of the box.
    dx, dy, dz = np.array(box_dimensions) / 2.0
    local_corners = np.array([
        [-dx, -dy, -dz],
        [dx, -dy, -dz],
        [dx, dy, -dz],
        [-dx, dy, -dz],
        [-dx, -dy, dz],
        [dx, -dy, dz],
        [dx, dy, dz],
        [-dx, dy, dz]
    ])

    # Rotate and translate the corners.
    global_corners = (rotation_matrix @ local_corners.T).T + np.array(box_center)

    # Define the 6 faces of the box (each as a list of 4 points).
    faces = [
        [global_corners[i] for i in [0, 1, 2, 3]],  # bottom face
        [global_corners[i] for i in [4, 5, 6, 7]],  # top face
        [global_corners[i] for i in [0, 1, 5, 4]],  # side face
        [global_corners[i] for i in [1, 2, 6, 5]],  # side face
        [global_corners[i] for i in [2, 3, 7, 6]],  # side face
        [global_corners[i] for i in [3, 0, 4, 7]]  # side face
    ]

    # Create a Poly3DCollection for the faces.
    poly3d_collection = Poly3DCollection(faces, facecolors=face_color,
                                         linewidths=1, edgecolors='k',
                                         alpha=face_opacity)
    ax.add_collection3d(poly3d_collection)

    return poly3d_collection


def plot_sphere(ax, center, radius, face_color='r', face_opacity=0.3):
    # Generate a unit sphere
    u = np.linspace(0, 2 * np.pi, 100)
    v = np.linspace(0, np.pi, 100)
    x = radius * np.outer(np.cos(u), np.sin(v))
    y = radius * np.outer(np.sin(u), np.sin(v))
    z = radius * np.outer(np.ones(np.size(u)), np.cos(v))

    # Translate the sphere to the desired center point
    x = x + center[0]
    y = y + center[1]
    z = z + center[2]

    # Plot the sphere
    plots = ax.plot_surface(x, y, z, color=face_color, alpha=face_opacity, edgecolor='none')

    # Adjust the appearance of the plot
    ax.set_box_aspect([1, 1, 1])  # Equal aspect ratio for all axes
    ax.set_xlabel('X-axis')
    ax.set_ylabel('Y-axis')
    ax.set_zlabel('Z-axis')
    return plots


def plot_links_modeling(ax, link_segments, final_link):
    required_segment_keys = ['aSegmentV0', 'aSegmentV1', 'dSegmentV0', 'dSegmentV1', 'radius']
    required_final_link_keys = ['center', 'dimension', 'orientation']

    plots = []
    for i, segment in enumerate(link_segments):
        # Check if segment dictionary contains required keys
        if not all(key in segment for key in required_segment_keys):
            raise ValueError(f"Segment {i} doesn't contain all required keys.")

        # Check if the values are correct and not NaN and then create the plots
        if not np.isnan(segment['aSegmentV0']).any() and not np.isnan(segment['aSegmentV1']).any():
            plots.append(plot_cylinder(ax, segment['aSegmentV0'], segment['aSegmentV1'], segment['radius']))
            plots.append(plot_sphere(ax, segment['aSegmentV0'], segment['radius']))
        if not np.isnan(segment['dSegmentV0']).any() and not np.isnan(segment['dSegmentV1']).any():
            plots.append(plot_cylinder(ax, segment['dSegmentV0'], segment['dSegmentV1'], segment['radius']))
            plots.append(plot_sphere(ax, segment['dSegmentV0'], segment['radius']))

    # Check if final_link dictionary contains required keys
    if not all(key in final_link for key in required_final_link_keys):
        raise ValueError(f"Final link doesn't contain all required keys.")

    plots.append(plot_box(ax, final_link['center'], final_link['dimension'], final_link['orientation'], 'yellow', 0.3))

    return plots


def plot_line_swept_spheres(ax, link_segments):
    plots = []
    for i, segment in enumerate(link_segments):
        if i == len(link_segments) - 1:
            # Handle the last segment separately (Tool Segment)
            if not np.isnan(segment['Tool_V0']).any() and not np.isnan(segment['Tool_V1']).any():
                plots.append(plot_cylinder(ax, segment['Tool_V0'], segment['Tool_V1'],
                                           link_segments[i - 1]['radius']))  # Using radius of the last link
                plots.append(
                    plot_sphere(ax, segment['Tool_V0'], link_segments[i - 1]['radius']))  # Radius of the last link


        else:
            # Plot cylinder for 'a' segment
            if not np.isnan(segment['aSegmentV0']).any() and not np.isnan(segment['aSegmentV1']).any():
                plots.append(plot_cylinder(ax, segment['aSegmentV0'], segment['aSegmentV1'], segment['radius']))
                plots.append(plot_sphere(ax, segment['aSegmentV0'], segment['radius']))

            # Plot cylinder for 'd' segment
            if not np.isnan(segment['dSegmentV0']).any() and not np.isnan(segment['dSegmentV1']).any():
                plots.append(plot_cylinder(ax, segment['dSegmentV0'], segment['dSegmentV1'], segment['radius']))
                plots.append(plot_sphere(ax, segment['dSegmentV0'], segment['radius']))
    return plots


def plot_line_segments(ax, link_segments, line_width=2):
    # Define a colormap
    colormap = plt.cm.get_cmap('hsv', len(link_segments))
    plots = []
    # Start plotting
    ax.set_xlabel('X-axis')
    ax.set_ylabel('Y-axis')
    ax.set_zlabel('Z-axis')
    ax.set_box_aspect([1, 1, 1])
    ax.grid(True)
    ax.autoscale(enable=True, axis='both', tight=None)

    for i, segment in enumerate(link_segments):
        # Get the color for the current segment
        color = colormap(i)
        if i == len(link_segments) - 1:
            # Handle the last segment separately (Tool Segment)
            if not np.isnan(segment['Tool_V0']).any() and not np.isnan(segment['Tool_V1']).any():
                plot = ax.plot([segment['Tool_V0'][0], segment['Tool_V1'][0]],
                               [segment['Tool_V0'][1], segment['Tool_V1'][1]],
                               [segment['Tool_V0'][2], segment['Tool_V1'][2]],
                               color=color, linestyle='-', linewidth=line_width)
                plots.extend(plot)
        else:
            # Plot 'a' segment if it exists
            if not np.isnan(segment['aSegmentV0']).any() and not np.isnan(segment['aSegmentV1']).any():
                plot = ax.plot([segment['aSegmentV0'][0], segment['aSegmentV1'][0]],
                               [segment['aSegmentV0'][1], segment['aSegmentV1'][1]],
                               [segment['aSegmentV0'][2], segment['aSegmentV1'][2]],
                               color=color, linestyle='-', linewidth=line_width)  # Unique color for this line segment
                plots.extend(plot)
            # Plot 'd' segment if it exists
            if not np.isnan(segment['dSegmentV0']).any() and not np.isnan(segment['dSegmentV1']).any():
                plot = ax.plot([segment['dSegmentV0'][0], segment['dSegmentV1'][0]],
                               [segment['dSegmentV0'][1], segment['dSegmentV1'][1]],
                               [segment['dSegmentV0'][2], segment['dSegmentV1'][2]],
                               color=color, linestyle='-', linewidth=line_width)  # Same color as 'a' segment
                plots.extend(plot)
    return plots


# Helper function to flatten a list of lists
def flatten(l):
    for item in l:
        if isinstance(item, list):
            for subitem in flatten(item):
                yield subitem
        else:
            yield item


# %%
class serial_chain_robot:
    def __init__(self, mdh_matrix, tool_translation, tool_rot, number_joints):
        self.real_time_start = None
        self.mdh_matrix = mdh_matrix
        self.End_tool_transformation = transl(tool_translation) @ rpy2tr(roll=math.radians(tool_rot[0]),
                                                                         pitch=math.radians(tool_rot[1]),
                                                                         yaw=math.radians(tool_rot[2]), order="zyx")
        self.number_joints = number_joints
        self.fig = plt.figure()
        self.ax = self.fig.add_subplot(111, projection='3d')
        self.lineSweptPlot = None
        self.elapsed_text = None
        self.line_segment_plot = None
        self.x_lim = [-1, 1]
        self.y_lim = [-1, 1]
        self.z_lim = [-0.3, 1]
        self.anim = None
        self.robot_links_plot = None

    def transform_MDH(self, a, alpha, d, theta):

        """
        Compute individual transformation matrices using modified DH parameters.

        Parameters:
        a (float): Link length
        alpha (float): Link twist
        d (float): Link offset
        theta (float): Joint angle

        Returns:
        np.ndarray: Transformation matrix
        """
        transform = np.array([[np.cos(theta), -np.sin(theta), 0, a],
                              [np.sin(theta) * np.cos(alpha), np.cos(theta) * np.cos(alpha), -np.sin(alpha),
                               -np.sin(alpha) * d],
                              [np.sin(theta) * np.sin(alpha), np.cos(theta) * np.sin(alpha), np.cos(alpha),
                               np.cos(alpha) * d],
                              [0, 0, 0, 1]])
        return transform

    def forward_kinematics_link(self, joint_positions, ith_link):
        T = np.eye(4)
        for i in range(ith_link):
            T_i = self.transform_MDH(
                d=self.mdh_matrix[i][1],
                a=self.mdh_matrix[i][2],
                alpha=math.radians(self.mdh_matrix[i][3]),
                theta=joint_positions[i])
            T = T @ T_i
        return T

    def forward_kinematics_tool(self, joint_positions):
        transform = self.forward_kinematics_link(joint_positions, self.number_joints) @ self.End_tool_transformation
        return transform

    def create_link_segments(self, radius, joint_angles):
        # Initialize line segments dictionary array
        link_segments = [{'aSegmentV0': np.array([np.nan, np.nan, np.nan]),
                          'aSegmentV1': np.array([np.nan, np.nan, np.nan]),
                          'dSegmentV0': np.array([np.nan, np.nan, np.nan]),
                          'dSegmentV1': np.array([np.nan, np.nan, np.nan]),
                          'radius': np.nan,
                          'radiusJoint': np.nan} for x in range(self.mdh_matrix.shape[0])]
        link_segments.append({'Tool_V0': np.array([np.nan, np.nan, np.nan]),
                              'Tool_V1': np.array([np.nan, np.nan, np.nan]),
                              'radius': np.nan})
        # Assume base configuration
        prev_transform = np.eye(4)  # Identity matrix

        for i in range(1, self.mdh_matrix.shape[0] + 1):
            # Transformation from the base to the previous link's coordinate frame
            if i == 1:
                prev_transform = np.eye(4)  # Update this method based on your robot class
            else:
                prev_transform = self.forward_kinematics_link(joint_angles, i - 1)

            # Initialize aTransform to safely handle any execution path
            a_transform = np.eye(4)

            # Check if 'a' is not zero and calculate 'a' segment
            if self.mdh_matrix[i - 1, 2] != 0:
                link_segments[i - 1]['aSegmentV0'] = prev_transform[0:3, 3]
                a_transform = prev_transform @ transl(
                    np.asarray([self.mdh_matrix[i - 1, 2], 0, 0]))  # Translate along x by 'a'
                link_segments[i - 1]['aSegmentV1'] = a_transform[0:3, 3]

            # Check if 'd' is not zero and calculate 'd' segment
            if self.mdh_matrix[i - 1, 1] != 0:
                if self.mdh_matrix[i - 1, 2] != 0:
                    link_segments[i - 1]['dSegmentV0'] = link_segments[i - 1]['aSegmentV1']
                    d_transform = a_transform @ rpy2tr(roll=math.radians(self.mdh_matrix[i - 1, 3]), pitch=0, yaw=0,
                                                       order="zyx") @ transl(
                        np.asarray([0, 0, self.mdh_matrix[i - 1, 1]]))
                    link_segments[i - 1]['dSegmentV1'] = d_transform[0:3, 3]

                else:
                    link_segments[i - 1]['dSegmentV0'] = prev_transform[0:3, 3]
                    d_transform = prev_transform @ rpy2tr(roll=math.radians(self.mdh_matrix[i - 1, 3]), pitch=0, yaw=0,
                                                          order="zyx") @ transl(
                        np.asarray([0, 0, self.mdh_matrix[i - 1, 1]]))
                    link_segments[i - 1]['dSegmentV1'] = d_transform[0:3, 3]
                if i == self.mdh_matrix.shape[0]:
                    link_segments[i]['Tool_V0'] = link_segments[i - 1]['dSegmentV1']
                    tcp_transform = d_transform @ self.End_tool_transformation
                    link_segments[i]['Tool_V1'] = tcp_transform[0:3, 3]
            else:
                if i == self.mdh_matrix.shape[0]:
                    link_segments[i]['Tool_V0'] = link_segments[i - 1]['aSegmentV1']
                    tcp_transform = a_transform @ self.End_tool_transformation
                    link_segments[i]['Tool_V1'] = tcp_transform[0:3, 3]
            link_segments[i]['radius'] = radius[i-1]

        return link_segments

    def create_link_segments_new(
            self,
            radius_links: np.ndarray,  # len = #links+1   (same as before)
            radius_joints: np.ndarray,  # len = #links     (new)
            final_link_dimensions: np.ndarray,  # (Lx, Ly, Lz) for the tool body (new)
            joint_angles: np.ndarray  # len = #joints
    ):
        """
        Build geometric primitives that approximate the robot for
        collision/visualisation, closely mirroring the MATLAB helper
        `createLineSegmentsWithJoints`.

        Returns
        -------
        link_segments : list[dict]
            One dict per kinematic link *except* the tool body, plus an
            extra dict for the TCP line segment.  Keys:
              • aSegmentV0, aSegmentV1
              • dSegmentV0, dSegmentV1
              • radius        (capsule radius for the link body)
              • radiusJoint   (capsule radius for the joint at the *inboard* end)
        final_link_box : dict
            Keys:
              • center           : 3-vector centre of the box
              • dimension        : copy of `final_link_dimensions`
              • orientation      : 3×3 rotation matrix of the TCP
              • finalV0, finalV1 : end-points of the tool-centre line (from end effector to TCP)
        """

        n_links = self.mdh_matrix.shape[0]  # == number of joints
        # --- allocate ----------------------------------------------------------
        link_segments = [{
            'aSegmentV0': np.full(3, np.nan),
            'aSegmentV1': np.full(3, np.nan),
            'dSegmentV0': np.full(3, np.nan),
            'dSegmentV1': np.full(3, np.nan),
            'radius': np.nan,
            'radiusJoint': np.nan
        } for _ in range(n_links)]

        # # tool “link” (line only – its body is the box below)
        # tool_dict = {'Tool_V0': np.full(3, np.nan),
        #              'Tool_V1': np.full(3, np.nan),
        #              'radius': radius_links[-1]}
        # link_segments.append(tool_dict)
        tcp_v0 = []
        tcp_v1 = []
        # --- iterate over conventional links ----------------------------------
        for i in range(1, n_links + 1):
            # transformation to the *previous* link’s frame
            t_prev = np.eye(4) if i == 1 else self.forward_kinematics_link(joint_angles, i - 1)
            a_i, d_i, alpha_i = self.mdh_matrix[i - 1, 2], self.mdh_matrix[i - 1, 1], np.radians(
                self.mdh_matrix[i - 1, 3])

            # convenience vars
            seg = link_segments[i - 1]
            seg['radius'] = radius_links[i - 1]
            seg['radiusJoint'] = radius_joints[i - 1]

            # --- “a” segment (along x-axis) ------------------------------------
            if not np.isclose(a_i, 0.0):
                seg['aSegmentV0'] = t_prev[:3, 3]
                t_a = t_prev @ transl([a_i, 0, 0])
                seg['aSegmentV1'] = t_a[:3, 3]
            else:
                t_a = t_prev  # nothing happened

            # --- “d” segment (along z-axis after alpha) ------------------------
            if not np.isclose(d_i, 0.0):
                if not np.isclose(a_i, 0.0):
                    seg['dSegmentV0'] = seg['aSegmentV1']
                else:
                    seg['dSegmentV0'] = t_prev[:3, 3]

                t_d = (t_a @ rpy2tr(roll=alpha_i, pitch=0, yaw=0, order="zyx")
                       @ transl([0, 0, d_i]))
                seg['dSegmentV1'] = t_d[:3, 3]
            # -------------------------------------------------------------------

            # --- Compute tool line for the *last* conventional link -----------
            if i == n_links:
                tcp_v0 = seg['dSegmentV1'] if not np.isnan(seg['dSegmentV1']).any() else seg[
                    'aSegmentV1']
                tcp_t = t_d @ self.End_tool_transformation
                tcp_v1 = tcp_t[:3, 3]

        # ----------------------------------------------------------------------
        # --------------------  FINAL LINK  BOX  -------------------------------
        final_v0 = tcp_v0
        final_v1 = tcp_v1
        tcp_t = self.forward_kinematics_tool(joint_angles)
        r_tcp = tcp_t[:3, :3]

        dim = np.asarray(final_link_dimensions, dtype=float)
        dist_vec = final_v1 - final_v0
        dist_unit = dist_vec / np.linalg.norm(dist_vec)
        tol = 1e-4

        # project onto TCP axes
        dots = dist_unit @ r_tcp  # [dx dy dz] in TCP coords
        axis_idx = np.argmax(np.abs(dots))  # 0→x, 1→y, 2→z
        if np.abs(dots[axis_idx]) < tol:
            raise ValueError("Cannot identify dominant axis for final link.")

        half_len = 0.5 * dim[axis_idx]
        center = final_v0 + dist_unit * half_len

        final_link_box = {
            'center': center,
            'dimension': dim,
            'orientation': Rotation.from_matrix(r_tcp).as_quat(),
            'finalV0': final_v0,
            'finalV1': final_v1
        }

        return link_segments, final_link_box

    def init_plot(self, x_lim=None, y_lim=None, z_lim=None):
        if z_lim is None:
            self.z_lim = [-0.3, 1]
        else:
            self.z_lim = z_lim

        if x_lim is None:
            self.x_lim = [-1, 1]
        else:
            self.x_lim = x_lim

        if y_lim is None:
            self.y_lim = [-1, 1]
        else:
            self.y_lim = y_lim

        self.ax.set_title('Robot Visualization')
        self.ax.set_xlim(self.x_lim)
        self.ax.set_ylim(self.y_lim)
        self.ax.set_zlim(self.z_lim)

        self.elapsed_text = self.ax.text2D(0.05, 0.95, 'Elapsed Time: 0s', transform=self.ax.transAxes,
                                           verticalalignment='top', fontsize=12)
        self.frame_text = self.ax.text2D(0.05, 0.90, 'Frame: 0', transform=self.ax.transAxes,
                                         verticalalignment='top', fontsize=12)
        self.line_segment_plot = None
        self.lineSweptPlot = None

    def plot(self, joint_angles_matrix, trajectory_time, ts, waypoints=None, obstacles=None, mode=True, radius=None,
             x_lim=None, y_lim=None, z_lim=None):
        self.init_plot(x_lim, y_lim, z_lim)
        if radius is None:
            radius = np.ones(joint_angles_matrix.shape[0] + 1) * 0.1

        # Plot waypoints if available
        if waypoints is not None:
            self.ax.scatter(waypoints[0, :], waypoints[1, :], waypoints[2, :], c='g', marker='o', linewidth=2)

        # Plot trajectory if mode is 2
        if mode == True and waypoints is not None:
            self.ax.plot(waypoints[0, :], waypoints[1, :], waypoints[2, :], '.-', color='black')

        # Plot obstacles if available
        if obstacles is not None:
            for obstacle in obstacles:
                plot_sphere(self.ax, obstacle['center'], obstacle['dimensions'][0])

        real_time_start = time.time()
        idx = 0
        while idx < joint_angles_matrix.shape[1]:
            real_time_elapsed = (time.time() - real_time_start) * 1000  # Convert to milliseconds
            expected_time = trajectory_time[idx] * 1000  # Convert to milliseconds
            if real_time_elapsed > expected_time:
                idx_increase = (real_time_elapsed - expected_time) // (ts * 1000)
                idx += int(idx_increase)

            elif real_time_elapsed < expected_time:
                time.sleep((expected_time - real_time_elapsed) / 1000)  # Convert ms back to seconds

            if idx >= joint_angles_matrix.shape[1]:
                break
            joint_angles = joint_angles_matrix[:, idx]

            line_segments_robot = self.create_link_segments(radius, joint_angles)
            # Remove old artists and clear the lists for the next iteration
            if self.lineSweptPlot is not None:
                for artist in self.lineSweptPlot:
                    artist.remove()
                self.lineSweptPlot.clear()
            else:
                self.lineSweptPlot = []

            if self.line_segment_plot is not None:
                for artist in self.line_segment_plot:
                    artist.remove()
                self.line_segment_plot.clear()
            else:
                self.line_segment_plot = []

            # Plot the updated lineSweptPlot and line_segment_plot
            self.lineSweptPlot = list(flatten(plot_line_swept_spheres(self.ax, line_segments_robot)))
            self.line_segment_plot = list(flatten(plot_line_segments(self.ax, line_segments_robot, line_width=5)))
            if z_lim is None:
                z_lim = [-0.3, 1]
            if x_lim is None:
                x_lim = [-1, 1]
            if y_lim is None:
                y_lim = [-1, 1]

            self.ax.set_xlim(x_lim)
            self.ax.set_ylim(y_lim)
            self.ax.set_zlim(z_lim)
            elapsed_time = time.time() - real_time_start
            self.elapsed_text.set_text(f'Elapsed Time: {elapsed_time:.2f}s')
            plt.draw()
            plt.pause(ts)
            idx += 1

        plt.show(block=True)

    def update_animate(self, frame, joint_angles_matrix, trajectory_time, ts, radius, radius_joints,
                       final_link_dimensions):
        real_time_elapsed = (time.time() - self.real_time_start) * 1000  # Convert to milliseconds
        expected_time = trajectory_time[frame] * 1000  # Convert to milliseconds
        if real_time_elapsed > expected_time:
            idx_increase = (real_time_elapsed - expected_time) // (ts * 1000)
            frame_idx = int(frame + idx_increase)
            if frame_idx >= joint_angles_matrix.shape[1]:
                self.anim.event_source.stop()
                frame_idx = joint_angles_matrix.shape[1] - 1
                # Prevent going out of bounds
        elif real_time_elapsed < expected_time:
            time.sleep((expected_time - real_time_elapsed) / 1000)
            frame_idx = frame
        else:
            frame_idx = frame
        # Convert ms back to seconds

        joint_angles = joint_angles_matrix[:, frame_idx]

        line_segments_robot = self.create_link_segments(radius, joint_angles)
        robot_links, final_link = self.create_link_segments_new(radius, radius_joints, final_link_dimensions,
                                                                joint_angles)
        # Remove old artists and clear the lists for the next iteration

        if self.robot_links_plot is not None:
            for artist in self.robot_links_plot:
                artist.remove()
            self.robot_links_plot.clear()
        else:
            self.robot_links_plot = []
        # if self.lineSweptPlot is not None:
        #     for artist in self.lineSweptPlot:
        #         artist.remove()
        #     self.lineSweptPlot.clear()
        # else:
        #     self.lineSweptPlot = []

        if self.line_segment_plot is not None:
            for artist in self.line_segment_plot:
                artist.remove()
            self.line_segment_plot.clear()
        else:
            self.line_segment_plot = []

        # Plot the updated lineSweptPlot and line_segment_plot
        # self.lineSweptPlot = list(flatten(plot_line_swept_spheres(self.ax, line_segments_robot)))
        self.robot_links_plot = list(flatten(plot_links_modeling(self.ax, robot_links, final_link)))
        self.line_segment_plot = list(flatten(plot_line_segments(self.ax, line_segments_robot, line_width=5)))

        self.ax.set_xlim(self.x_lim)
        self.ax.set_ylim(self.y_lim)
        self.ax.set_zlim(self.z_lim)

        if frame_idx <= joint_angles_matrix.shape[1]:
            elapsed_time = time.time() - self.real_time_start
            self.elapsed_text.set_text(f'Elapsed Time: {elapsed_time:.2f}s')
            self.frame_text.set_text(f'Frame: {frame_idx}')
        else:
            self.anim.event_source.stop()
            return

    def plot_animate(self, joint_angles_matrix, trajectory_time, sample_time, waypoints=None, obstacles_for_plot=None,
                     mode=True,
                     radius=None, radius_joints=None, final_link_dimensions=None,
                     x_lim=None, y_lim=None, z_lim=None):
        self.init_plot(x_lim, y_lim, z_lim)
        if radius is None:
            radius = np.ones(joint_angles_matrix.shape[0] + 1) * 0.1

        # Plot waypoints if available
        if waypoints is not None:
            self.ax.scatter(waypoints[0, :], waypoints[1, :], waypoints[2, :], c='g', marker='o', linewidth=2)

        # Plot trajectory if mode is 2
        if mode == True and waypoints is not None:
            self.ax.plot(waypoints[0, :], waypoints[1, :], waypoints[2, :], '.-', color='black')

        # Plot obstacles_for_plot if available
        if obstacles_for_plot is not None:
            for obstacle in obstacles_for_plot:
                if obstacle['type'] == "Sphere":
                    plot_sphere(self.ax, obstacle['center'], obstacle['dimensions'][0])
                elif obstacle['type'] == "Box":
                    plot_box(self.ax, obstacle['center'], obstacle['dimensions'], obstacle['orientation'])
                elif obstacle['type'] == "Cylinder":
                    plot_cylinder_new(self.ax, obstacle['dimensions'][0], obstacle['dimensions'][1], obstacle['center'],
                                      obstacle['axis'])

        self.real_time_start = time.time()

        frames = np.arange(joint_angles_matrix.shape[1])
        self.anim = FuncAnimation(plt.gcf(), self.update_animate, frames=frames,
                                  fargs=(joint_angles_matrix, trajectory_time, sample_time, radius, radius_joints,
                                         final_link_dimensions),
                                  interval=sample_time * 1000)
        plt.show(block=True)


# %%


def read_json_file(filename):
    with open(filename, 'r') as file:
        # Parse the JSON data and convert to a dictionary
        data_dict = json.load(file)
    return data_dict


# data = pd.read_csv("../outputDesiredJointsDemoPartTwo.csv", header=None)
data = pd.read_csv("../outputDemo/robotJointPositions.csv", header=None)
print(data.shape)
joint_values = data.values
print(joint_values.shape)
robot_config = read_json_file("../config/configDemo/pandaRobot.json")
Trajectory_config = read_json_file("../config/configDemo/trajectoryConfig.json")
obstacles_config = read_json_file("../config/configDemo/Obstacles.json")
obstacles_data = obstacles_config["Obstacles"]

obstacles = []
for obs_name, obs_details in obstacles_data.items():
    obstacle = obs_details  # This is a dictionary of obstacle detail
    obstacles.append(obstacle)

# # Create Obstacles dictionary array manually here in the code
# obstacles = [
#     # {"type": "Sphere", "center": [0.5545, 0.20, 0.5211], "dimensions": [0.04, 0.04, 0.04]},
#     # {"type": "Sphere", "center": [0.5545, 0.20, 0.3211], "dimensions": [0.05, 0.05, 0.05]},
#     # {"type": "Box", "center": [0.600, 0.0, 0.1211], "dimensions": [0.04, 0.04, 0.12]}
# ]

joint_values_beginning = np.asarray(robot_config["Joints"]["jointPositionAtBeginning"])
mdh_params = robot_config["mdhParameters"]
# new_joint_values = joint_values[:, 0:10000:50]
radius_links = np.asarray(robot_config["ObstacleAvoidanceParameters"]["radiusLinks"])
radius_joints = np.asarray(robot_config["ObstacleAvoidanceParameters"]["radiusJoints"])
final_link_box_dimensions = np.asarray(robot_config["ObstacleAvoidanceParameters"]["finalLinkDimensions"])
# %%
tool_rot = robot_config["displacementEEtoTCP"]["rotationXYZ"]
tool_translation = robot_config["displacementEEtoTCP"]["translation"]
# tool_transformation = SE3(tool_translation) * SE3.RPY(tool_rot, order="zyx", unit="deg")
mdh_matrix = np.array(mdh_params)

# %%
robot_Serial_chain = serial_chain_robot(mdh_matrix, tool_translation, tool_rot, mdh_matrix.shape[0])
# %%

print(robot_Serial_chain.forward_kinematics_link(joint_values[:, 0], 7))
# %%

# link_segments_plot = robot_Serial_chain.create_link_segments(radius_Links, joint_values[:, 0])
# %%

# %%
Waypoints = np.asarray(Trajectory_config['Waypoints'])
print(Waypoints)

ts = Trajectory_config['trajectorySampleTime']
waypoint_times = np.asarray(Trajectory_config['waypointTimes'])
trajectory_time_array = np.arange(waypoint_times[0], waypoint_times[-1] + ts, ts)

# fig = plt.figure()
# ax = fig.add_subplot(111, projection='3d')
#
# plot_box(ax,[0.5545,0,0.5945],[0.06,0.14,0.11],[0,1,0,0])
#
# link_segments_plot= robot_Serial_chain.create_link_segments([0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1],[0, 0, 0, -math.pi / 2, 0, math.pi/ 2, math.pi/4])
# plot_line_swept_spheres(ax, link_segments_plot)
# %%
# robot_Serial_chain.plot(joint_values, trajectory_time_array, ts, Waypoints)
robot_Serial_chain.plot_animate(joint_values, trajectory_time_array, ts, Waypoints, obstacles, radius=radius_links,
                                radius_joints=radius_joints, final_link_dimensions= final_link_box_dimensions)
l = 1 + 7

# pyplot = rtb.backends.PyPlot()  # create a PyPlot backend
#
# pyplot.add(robot)  # add the robot to the backend
#
# robot.q = joint_values[:,0]  # set the robot configuration
#
# pyplot.step()  # update the backend and graphical view

# from roboticstoolbox.backends import PyPlot
#
# pyplot = PyPlot.PyPlot()
# env = pyplot.launch()  # Ensure you launch the backend properly
# env.add(robot)
# # pyplot.add(robot)
#
# robot.q = joint_values[:, 0]
# env.add(robot)
# env.step()
# pyplot.step()

# print("Robot visualization complete.")
# plt.close('all')
# Animate the robot movement
# for q in joint_values:
#     robot.q = q
#     pyplot.step()
#     pyplot.hold()
#
# # Block and display the plot (fig stays open)
# pyplot.hold()
# Create a plot to visualize the animation
# fig, ax = plt.subplots()
# ax = fig.add_subplot(111, projection='3d')
# ax.set_xlim([-1, 1])
# ax.set_ylim([-1, 1])
# ax.set_zlim([0, 1])
#
# # Animate the robot movement
# ani = robot.plot(joint_values.T,dt=0.001, block=False, movie="robot_motion.html")
#
# # Show the animation
# plt.show()
#
# rtp.jt

# See PyCharm help at https://www.jetbrains.com/help/pycharm/
# comment
