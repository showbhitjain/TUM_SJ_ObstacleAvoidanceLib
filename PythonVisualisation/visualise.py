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

matplotlib.use('QT5Agg')


# %%
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
                          'radius': np.nan} for x in range(self.mdh_matrix.shape[0])]
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
            link_segments[i]['radius'] = radius[i]

        return link_segments

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

    def update_animate(self, frame, joint_angles_matrix, trajectory_time, ts):
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

    def plot_animate(self, joint_angles_matrix, trajectory_time, ts, waypoints=None, obstacles=None, mode=True,
                     radius=None,
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

        self.real_time_start = time.time()

        frames = np.arange(joint_angles_matrix.shape[1])
        self.anim = FuncAnimation(plt.gcf(), self.update_animate, frames=frames,
                                  fargs=(joint_angles_matrix, trajectory_time, ts),
                                  interval=ts * 1000)
        plt.show(block=True)


# %%


def read_json_file(filename):
    with open(filename, 'r') as file:
        # Parse the JSON data and convert to a dictionary
        data_dict = json.load(file)
    return data_dict


data = pd.read_csv("../outputDesiredJoints.csv", header=None)
print(data.shape)
joint_values = data.values
print(joint_values.shape)
robot_config = read_json_file("../config/pandaRobot.json")
Trajectory_config = read_json_file("../config/trajectoryConfig.json")
mdh_params = robot_config["mdhParameters"]
# new_joint_values = joint_values[:, 0:10000:50]

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
radius = np.ones(8) * 0.1
link_segments_plot = robot_Serial_chain.create_link_segments(radius, joint_values[:, 0])
# %%

# %%
Waypoints = np.asarray(Trajectory_config['Waypoints'])
ts = Trajectory_config['trajectorySampleTime']
waypoint_times = np.asarray(Trajectory_config['waypointTimes'])
trajectory_time = np.arange(waypoint_times[0], waypoint_times[-1] + ts, ts)

# %%
# robot_Serial_chain.plot(joint_values, trajectory_time, ts, Waypoints)
robot_Serial_chain.plot_animate(joint_values, trajectory_time, ts, Waypoints)
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
