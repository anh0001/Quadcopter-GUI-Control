import tkinter as tk
from tkinter import ttk
import serial.tools.list_ports
import serial

# Initialize main window
window = tk.Tk()
window.title("QUAD-C-V1.0")
window.geometry("700x500")
window.configure(background="light grey")

# Define variables for roll, pitch, and yaw data
roll_data = tk.StringVar(value="0.0")
pitch_data = tk.StringVar(value="0.0")
yaw_data = tk.StringVar(value="0.0")
throttle_data = tk.StringVar(value="0.0")

# Create header frame
header_frame = tk.Frame(window, bg="#F5F5F5")
header_frame.pack(fill=tk.X)

# Header label
header_label = tk.Label(
    header_frame,
    text="QUAD-C",
    font=("Arial", 20, "bold italic"),
    bg="White",
    fg="Black",
    justify="left",
)
header_label.pack(side="left", padx=20, pady=15)

# Define variables for roll, pitch, and yaw data
roll_data = tk.StringVar(value="0.0")
pitch_data = tk.StringVar(value="0.0")
yaw_data = tk.StringVar(value="0.0")

pid_roll_data = tk.StringVar(value="0.0")
pid_pitch_data = tk.StringVar(value="0.0")

status_data = tk.StringVar(value="Disarm")
throttle_data = tk.StringVar(value="0")

# Create data display frame
data_frame = tk.Frame(window)
data_frame.pack(fill=tk.BOTH, expand=True)

# Create labels and entry boxes for roll, pitch, and yaw

# Position labels above entry boxes
roll_label = tk.Label(data_frame, text="\nROLL", font=("Arial", 12, "bold"))
roll_entry = tk.Entry(data_frame, textvariable=roll_data, state="readonly", width=10)
roll_label.grid(row=2, column=0, padx=10, pady=5)
roll_entry.grid(row=3, column=0)

pitch_label = tk.Label(data_frame, text="\nPITCH", font=("Arial", 12, "bold"))
pitch_entry = tk.Entry(data_frame, textvariable=pitch_data, state="readonly", width=10)
pitch_label.grid(row=0, column=0, padx=10, pady=5)
pitch_entry.grid(row=1, column=0)

yaw_label = tk.Label(data_frame, text="\nYAW", font=("Arial", 12, "bold"))
yaw_entry = tk.Entry(data_frame, textvariable=yaw_data, state="readonly", width=10)
yaw_label.grid(row=4, column=0, padx=10, pady=5)
yaw_entry.grid(row=5, column=0)

# Create label and entry box for status
status_label = tk.Label(data_frame, text="\nSTATUS", font=("Arial", 12, "bold"))
status_entry = tk.Entry(data_frame, textvariable=status_data, state="readonly", width=20)

# Position status label and entry box
data_frame.grid_columnconfigure(7, weight=1)

# Move the "STATUS" label and entry box to the last column
status_label.grid(row=0, column=8, padx=50, pady=5, sticky="w")
status_entry.grid(row=1, column=8, padx=20, pady=5)

# Create label and entry box for throttle
throttle_label = tk.Label(data_frame, text="THROTTLE", font=("Arial", 10, "bold"))
throttle_entry = tk.Entry(data_frame, textvariable=throttle_data, state="readonly", width=10)

#throttle_slider.grid_forget()

# Position throttle label and entry box below roll
throttle_slider = ttk.Progressbar(
    data_frame,
    orient="horizontal",
    length=350,
    #mode="determinate",
    variable=throttle_data,
)

# Adjust layout for the slider
throttle_label.grid(row=6, column=0, padx=30, pady=5, sticky="w")
throttle_slider.grid(row=7, column=0, columnspan=5, padx=30, pady=0, sticky="w")

# Create label and entry box for PID Roll
pid_roll_label = tk.Label(data_frame, text="PID ROLL", font=("Arial", 12, "bold"))
pid_roll_entry = tk.Entry(data_frame, textvariable=pid_roll_data, state="readonly", width=10)

# Position PID Roll label and entry box below status
pid_roll_label.grid(row=0, column=5, padx=50, pady=5, sticky="w")
pid_roll_entry.grid(row=1, column=5, padx=20, pady=5)

# Create label and entry box for PID Pitch
pid_pitch_label = tk.Label(data_frame, text="PID PITCH", font=("Arial", 12, "bold"))
pid_pitch_entry = tk.Entry(data_frame, textvariable=pid_pitch_data, state="readonly", width=10)

# Position PID Pitch label and entry box below PID Roll
pid_pitch_label.grid(row=2, column=5, padx=50, pady=5, sticky="w")
pid_pitch_entry.grid(row=3, column=5, padx=20, pady=5)

# Define COM port list and baud rate options
com_port_list = ["COM1", "COM2", "COM3", "COM4"]  # Add your available COM ports
baud_rate_options = [9600, 115200, 57600]  # Add your desired baud rates

# Create drop-down menu for COM port
com_port_label = tk.Label(data_frame, text="COM PORT", font=("Arial", 10, "bold"))
com_port_variable = tk.StringVar(value=com_port_list[0])
com_port_dropdown = tk.OptionMenu(
    data_frame, com_port_variable, *com_port_list
)

# Create drop-down menu for baud rate
baud_rate_label = tk.Label(data_frame, text="BAUDRATE", font=("Arial", 10, "bold"))
baud_rate_variable = tk.IntVar(value=baud_rate_options[0])
baud_rate_dropdown = tk.OptionMenu(
    data_frame, baud_rate_variable, *baud_rate_options
)

# Move the COM port and baud rate dropdowns below the status label
com_port_label.grid(row=3, column=8, padx=50, pady=20, sticky="w")
com_port_dropdown.grid(row=4, column=8, padx=10, pady=5)
baud_rate_label.grid(row=5, column=8, padx=50, pady=20, sticky="w")
baud_rate_dropdown.grid(row=6, column=8, padx=10, pady=5)

# Adjust grid configuration for remaining elements
pid_roll_label.grid(row=2, column=1, padx=50, pady=5, sticky="w")
pid_roll_entry.grid(row=3, column=1, padx=20, pady=5)
pid_pitch_label.grid(row=0, column=1, padx=50, pady=5, sticky="w")
pid_pitch_entry.grid(row=1, column=1, padx=20, pady=5)

# Create footer frame
footer_frame = tk.Frame(window, bg="#ffffff", borderwidth=1, relief="groove")
footer_frame.pack(fill=tk.X, side="bottom")

# Create a container frame for both text labels
footer_text_frame = tk.Frame(footer_frame, bg="#ffffff")
footer_text_frame.pack(side="left", fill="both", expand=True)

# Create label for student information with two lines
student_info_label = tk.Label(
    footer_text_frame,
    text="4121600046  |  Junio Hangga Wicaksono\n4121600054  |  Achmad Bagus Okto Faerizqi",
    font=("Arial", 7),
    bg="#323232",
    justify="left",
)
student_info_label.pack(anchor="w", padx=10, pady=10)
student_info_label.configure(bg="#ffffff", fg="black")


# Create label for "Teknik Mekatronika"
mekatronika_label = tk.Label(
    footer_frame,
    text="Teknik Mekatronika\nPoliteknik Elektronika Negeri Surabaya",
    font=("Arial", 7),
    bg="#323232",
    justify="right",
)
mekatronika_label.pack(side="right", anchor="e", padx=10, pady=5)
mekatronika_label.configure(bg="#ffffff", fg="black")

# Declare global variables for buttons and serial connection
connect_button = None
disconnect_button = None
ser = None
connected = False

# Fungsi untuk membaca dan menampilkan data dari COM port
def read_serial_data():
    global connected, ser

    if connected:
        try:
            # Read data from the serial port
            data = ser.readline().decode().strip().split(' ')

            # Update roll, pitch, yaw, status, pid roll, pid pitch, and throttle data
            status_data.set(data[0])
            throttle_data.set(int(float(data[1])))
            roll_data.set(data[2])
            pitch_data.set(data[3])
            yaw_data.set(data[4])
            pid_roll_data.set(data[5])
            pid_pitch_data.set(data[6])

        except Exception as e:
            print(f"Error reading data from serial port: {e}")

    # Schedule the next update after 100 milliseconds
    window.after(100, read_serial_data)

# Function to connect to the COM port
def connect():
    global connected, ser, connect_button, disconnect_button
    com_port = com_port_variable.get()  # Get selected COM port
    baud_rate = baud_rate_variable.get()  # Get selected baud rate

    try:
        # Open the serial port
        ser = serial.Serial(com_port, baud_rate, timeout=1)
        connected = True

        # Enable/disable buttons accordingly
        connect_button.config(state="disabled")
        disconnect_button.config(state="normal")

    except Exception as e:
        print(f"Error connecting to {com_port}: {e}")

# Function to disconnect from the COM port
def disconnect():
    global connected, ser, connect_button, disconnect_button

    try:
        # Close the serial port
        if ser.is_open:
            ser.close()
        connected = False

        # Enable/disable buttons accordingly
        connect_button.config(state="normal")
        disconnect_button.config(state="disabled")

    except Exception as e:
        print(f"Error disconnecting: {e}")
        
def update_com_ports():
    available_ports = [port.device for port in serial.tools.list_ports.comports()]
    com_port_dropdown['menu'].delete(0, 'end')  # Clear existing menu

    for port in available_ports:
        com_port_dropdown['menu'].add_command(label=port, command=tk._setit(com_port_variable, port))

    # Set the default value to the first available port
    com_port_variable.set(available_ports[0] if available_ports else "")
    
# Update COM port dropdown initially
update_com_ports()

# Create buttons for connect and disconnect
connect_button = tk.Button(data_frame, text="Connect", command=connect)
disconnect_button = tk.Button(data_frame, text="Disconnect", state="disabled", command=disconnect)

# Position buttons next to COM port and baud rate dropdowns
connect_button.grid(row=6, column=5, padx=30, pady=5)
disconnect_button.grid(row=7, column=5, padx=30, pady=5)

# Update data every 100 milliseconds
window.after(100, read_serial_data)

# Start the main loop
window.mainloop()