
import socket
import threading
import tkinter as tk
from tkinter import ttk, messagebox, scrolledtext

SERVER_HOST_DEFAULT = "127.0.0.1"
SERVER_PORT_DEFAULT = 5000
RECV_BUFFER = 4096

class TCPClientGUI:
    def __init__(self, root):
        self.root = root
        root.title("Client TCP GUI")

        self.sock = None
        self.connected = False
        self.authenticated = False

        # --- Connection frame ---
        conn_frame = ttk.LabelFrame(root, text="Connection")
        conn_frame.grid(row=0, column=0, padx=8, pady=6, sticky="ew")

        ttk.Label(conn_frame, text="Serveur:").grid(row=0, column=0, sticky="w")
        self.entry_server = ttk.Entry(conn_frame, width=18)
        self.entry_server.grid(row=0, column=1, padx=4)
        self.entry_server.insert(0, SERVER_HOST_DEFAULT)

        ttk.Label(conn_frame, text="Port:").grid(row=0, column=2, sticky="w")
        self.entry_port = ttk.Entry(conn_frame, width=8)
        self.entry_port.grid(row=0, column=3, padx=4)
        self.entry_port.insert(0, str(SERVER_PORT_DEFAULT))

        self.btn_connect = ttk.Button(conn_frame, text="Connecter", command=self.connect_to_server)
        self.btn_connect.grid(row=0, column=4, padx=6)
    
        self.btn_disconnect = ttk.Button(conn_frame, text="Déconnecter", command=self.disconnect)
        self.btn_disconnect.grid(row=0, column=5, padx=6)
        self.btn_disconnect.state(["disabled"])

        # --- Login frame ---
        login_frame = ttk.LabelFrame(root, text="Connexion")
        login_frame.grid(row=1, column=0, padx=8, pady=6, sticky="ew")

        ttk.Label(login_frame, text="Nom d'utilisateur:").grid(row=0, column=0, sticky="w")
        self.entry_user = ttk.Entry(login_frame, width=16)
        self.entry_user.grid(row=0, column=1, padx=4)

        ttk.Label(login_frame, text="Mot de passe:").grid(row=0, column=2, sticky="w")
        self.entry_pass = ttk.Entry(login_frame, show="*", width=16)
        self.entry_pass.grid(row=0, column=3, padx=4)

        self.btn_login = ttk.Button(login_frame, text="Connexion", command=self.login)
        self.btn_login.grid(row=0, column=4, padx=6)
        self.btn_login.state(["disabled"])

        # --- Services frame ---
        svc_frame = ttk.LabelFrame(root, text="Services")
        svc_frame.grid(row=2, column=0, padx=8, pady=6, sticky="ew")

        self.btn_date = ttk.Button(svc_frame, text="Date / Heure (SERVICE 1)", command=lambda: self.request_service(1))
        self.btn_date.grid(row=0, column=0, padx=4, pady=4)
        self.btn_date.state(["disabled"])

        self.btn_list = ttk.Button(svc_frame, text="Liste fichiers (SERVICE 2)", command=lambda: self.request_service(2))
        self.btn_list.grid(row=0, column=1, padx=4, pady=4)
        self.btn_list.state(["disabled"])

        ttk.Label(svc_frame, text="Nom fichier:").grid(row=1, column=0, sticky="e")
        self.entry_filename = ttk.Entry(svc_frame, width=30)
        self.entry_filename.grid(row=1, column=1, columnspan=2, padx=4, sticky="w")

        self.btn_file = ttk.Button(svc_frame, text="Lire fichier (SERVICE 3)", command=lambda: self.request_service(3))
        self.btn_file.grid(row=1, column=3, padx=4, pady=4)
        self.btn_file.state(["disabled"])

        self.btn_time = ttk.Button(svc_frame, text="Durée connexion (SERVICE 4)", command=lambda: self.request_service(4))
        self.btn_time.grid(row=2, column=0, padx=4, pady=4)
        self.btn_time.state(["disabled"])

        self.btn_quit = ttk.Button(svc_frame, text="Quit (SERVICE 5)", command=lambda: self.request_service(5))
        self.btn_quit.grid(row=2, column=1, padx=4, pady=4)
        self.btn_quit.state(["disabled"])

        # --- Output frame ---
        out_frame = ttk.LabelFrame(root, text="Réponse du serveur")
        out_frame.grid(row=3, column=0, padx=8, pady=6, sticky="nsew")
        root.grid_rowconfigure(3, weight=1)
        root.grid_columnconfigure(0, weight=1)

        self.output = scrolledtext.ScrolledText(out_frame, width=80, height=20)
        self.output.pack(fill="both", expand=True)

        # status
        self.status_var = tk.StringVar(value="Déconnecté")
        ttk.Label(root, textvariable=self.status_var).grid(row=4, column=0, sticky="w", padx=8, pady=(0,8))

    # -------------------------
    # Connection / Login
    # -------------------------
    def connect_to_server(self):
        if self.connected:
            return
        server = self.entry_server.get().strip()
        port_text = self.entry_port.get().strip()
        try:
            port = int(port_text)
        except ValueError:
            messagebox.showerror("Erreur de port", "Le port doit être un nombre")
            return

        try:
            s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            s.settimeout(5.0)
            s.connect((server, port))
            s.settimeout(None)  # blocking mode for subsequent ops
            self.sock = s
            self.connected = True
            self.status_var.set(f"Connecté à {server}:{port}")
            self.btn_connect.state(["disabled"])
            self.btn_disconnect.state(["!disabled"])
            self.btn_login.state(["!disabled"])
            self.output_insert(f"Connecté à {server}:{port}\n")
        except Exception as e:
            messagebox.showerror("Erreur de connexion", str(e))
            self.output_insert(f"Erreur de connexion: {e}\n")

    def disconnect(self):
        if self.sock:
            try:
                self.sock.close()
            except Exception:
                pass
        self.sock = None
        self.connected = False
        self.authenticated = False
        self.btn_connect.state(["!disabled"])
        self.btn_disconnect.state(["disabled"])
        self.btn_login.state(["disabled"])
        self.disable_service_buttons()
        self.status_var.set("Disconnected")
        self.output_insert("Disconnected.\n")

    def login(self):
        if not self.connected or not self.sock:
            messagebox.showwarning("Non connecté", "Connectez-vous d'abord")
            return
        username = self.entry_user.get().strip()
        password = self.entry_pass.get().strip()
        if username == "" or password == "":
            messagebox.showwarning("Entrée", "Fournissez le nom d'utilisateur et le mot de passe")
            return

        # send LOGIN on the open connection
        try:
            msg = f"LOGIN {username} {password}\n"
            self.sock.sendall(msg.encode())
            # wait reply (use small timeout)
            self.sock.settimeout(5.0)
            resp = self._recv_once()
            self.sock.settimeout(None)
            if resp is None:
                messagebox.showerror("Connexion", "Pas de réponse du serveur")
                return
            resp_str = resp.strip()
            self.output_insert(f"Serveur: {resp_str}\n")
            if resp_str.startswith("OK"):
                self.authenticated = True
                messagebox.showinfo("Connexion", "Authentifié")
                self.enable_service_buttons()
                self.btn_login.state(["disabled"])
                self.status_var.set("Authentifié")
            else:
                self.authenticated = False
                messagebox.showerror("Connexion", "Échec de l'authentification")
        except Exception as e:
            messagebox.showerror("Erreur de connexion", str(e))
            self.output_insert(f"Login error: {e}\n")
            try:
                self.sock.settimeout(None)
            except Exception:
                pass

    # -------------------------
    # Services
    # -------------------------
    def request_service(self, code):
        if not self.connected or not self.sock:
            messagebox.showwarning("Non connecté", "Connectez-vous d'abord")
            return
        if not self.authenticated:
            messagebox.showwarning("Non authentifié", "Connectez-vous d'abord")
            return

        # run sending + receiving in a thread to avoid blocking UI
        t = threading.Thread(target=self._service_worker, args=(code,), daemon=True)
        t.start()

    def _service_worker(self, code):
        try:
            # Command format: "SERVICE n\n"
            cmd = f"SERVICE {code}\n"
            self.sock.sendall(cmd.encode())
            # If service 3, server expects filename next:
            if code == 3:
                filename = self.entry_filename.get().strip()
                if filename == "":
                    self.output_insert("Veuillez entrer un nom de fichier avant de demander le SERVICE 3\n")
                    return
                # send filename + newline
                self.sock.sendall((filename + "\n").encode())

            # read response(s) until a short timeout (server writes then returns to read)
            data = self._recv_until_pause()
            if data is None:
                self.output_insert("(no response)\n")
            else:
                self.output_insert("----- Début de la réponse -----\n")
                self.output_insert(data)
                if not data.endswith("\n"):
                    self.output_insert("\n")
                self.output_insert("----- Fin de la réponse -----\n")
            # If quit asked, then close locally
            if code == 5:
                # server will break and close socket; reflect that in GUI
                self.output_insert("Envoyé QUIT (SERVICE 5). Le serveur devrait fermer la connexion.\n")
                # close client socket locally
                try:
                    self.sock.close()
                except Exception:
                    pass
                self.connected = False
                self.authenticated = False
                self.btn_connect.state(["!disabled"])
                self.btn_disconnect.state(["disabled"])
                self.disable_service_buttons()
                self.status_var.set("Disconnected")
        except Exception as e:
            self.output_insert(f"Service error: {e}\n")

    # -------------------------
    # Helpers: receive
    # -------------------------
    def _recv_once(self):
        """Attempt to receive a single chunk. Returns string or None."""
        try:
            chunk = self.sock.recv(RECV_BUFFER)
            if not chunk:
                return None
            return chunk.decode(errors="replace")
        except socket.timeout:
            return None
        except Exception as e:
            self.output_insert(f"_recv_once error: {e}\n")
            return None

    def _recv_until_pause(self, pause=0.35, max_cycles=200):
        """
        Receives repeatedly until a short pause from the server (socket.timeout).
        pause: timeout in seconds for each recv attempt.
        Returns combined string or None if nothing received.
        """
        saved_timeout = None
        try:
            # set a small timeout so recv returns when server stops sending
            saved_timeout = self.sock.gettimeout()
            self.sock.settimeout(pause)
        except Exception:
            pass

        parts = []
        try:
            while True:
                try:
                    chunk = self.sock.recv(RECV_BUFFER)
                    if not chunk:
                        break
                    parts.append(chunk.decode(errors="replace"))
                    # if server likely sent everything in one write, loop will try again, then timeout
                except socket.timeout:
                    # pause observed -> likely end of response
                    break
                except Exception as e:
                    self.output_insert(f"_recv_until_pause error: {e}\n")
                    break
        finally:
            # restore timeout
            try:
                self.sock.settimeout(saved_timeout)
            except Exception:
                pass

        if len(parts) == 0:
            return None
        return "".join(parts)

    # -------------------------
    # UI helpers
    # -------------------------
    def output_insert(self, text):
        self.output.configure(state="normal")
        self.output.insert("end", text)
        self.output.see("end")
        self.output.configure(state="disabled")

    def enable_service_buttons(self):
        for btn in (self.btn_date, self.btn_list, self.btn_file, self.btn_time, self.btn_quit):
            btn.state(["!disabled"])

    def disable_service_buttons(self):
        for btn in (self.btn_date, self.btn_list, self.btn_file, self.btn_time, self.btn_quit):
            btn.state(["disabled"])

def main():
    root = tk.Tk()
    app = TCPClientGUI(root)
    root.mainloop()

if __name__ == "__main__":
    main()
