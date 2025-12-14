#include <gtk/gtk.h>
#include "core/services.h"

typedef struct {
    GtkApplication *app;
    GtkWidget *window;
    GtkWidget *stack;
    GtkWidget *header_bar;
    
    GtkWidget *login_page;
    GtkWidget *register_page;
    GtkWidget *dashboard_page;
    GtkWidget *data_entry_page;
    
    GtkWidget *login_user_entry;
    GtkWidget *login_mobile_entry;
    GtkWidget *login_password_entry;
    GtkWidget *login_status_label;
    
    GtkWidget *reg_name_entry;
    GtkWidget *reg_email_entry;
    GtkWidget *reg_mobile_entry;
    GtkWidget *reg_password_entry;
    GtkWidget *reg_confirm_entry;
    GtkWidget *reg_campus_combo;
    GtkWidget *reg_status_label;
    
    GtkWidget *dash_welcome_label;
    GtkWidget *dash_user_id_label;
    GtkWidget *dash_campus_label;
    GtkWidget *dash_email_label;
    GtkWidget *dash_mobile_label;
    GtkWidget *dash_data_view;
    
    Profile current_user;
    int is_logged_in;
} AppData;

static AppData *app_data = NULL;

static void show_message(GtkWidget *parent, const char *title, const char *message, GtkMessageType type) {
    GtkAlertDialog *dialog = gtk_alert_dialog_new("%s", message);
    gtk_alert_dialog_show(dialog, GTK_WINDOW(parent));
    g_object_unref(dialog);
}

static void switch_to_page(const char *page_name) {
    gtk_stack_set_visible_child_name(GTK_STACK(app_data->stack), page_name);
    
    if (strcmp(page_name, "login") == 0 || strcmp(page_name, "register") == 0) {
        gtk_header_bar_set_title_widget(GTK_HEADER_BAR(app_data->header_bar), 
            gtk_label_new(APP_NAME));
    } else if (strcmp(page_name, "dashboard") == 0) {
        char title[256];
        snprintf(title, sizeof(title), "%s - %s Dashboard", APP_NAME, 
                 get_campus_name(app_data->current_user.campusType));
        gtk_header_bar_set_title_widget(GTK_HEADER_BAR(app_data->header_bar),
            gtk_label_new(title));
    }
}

static void update_dashboard(void) {
    char buf[512];
    
    snprintf(buf, sizeof(buf), "Welcome, %s!", app_data->current_user.name);
    gtk_label_set_text(GTK_LABEL(app_data->dash_welcome_label), buf);
    
    snprintf(buf, sizeof(buf), "User ID: %s", app_data->current_user.userID);
    gtk_label_set_text(GTK_LABEL(app_data->dash_user_id_label), buf);
    
    snprintf(buf, sizeof(buf), "Campus: %s", get_campus_name(app_data->current_user.campusType));
    gtk_label_set_text(GTK_LABEL(app_data->dash_campus_label), buf);
    
    snprintf(buf, sizeof(buf), "Email: %s", app_data->current_user.email);
    gtk_label_set_text(GTK_LABEL(app_data->dash_email_label), buf);
    
    snprintf(buf, sizeof(buf), "Mobile: %s", app_data->current_user.mobile);
    gtk_label_set_text(GTK_LABEL(app_data->dash_mobile_label), buf);
}

static void on_login_clicked(GtkButton *button, gpointer user_data) {
    const char *userID = gtk_editable_get_text(GTK_EDITABLE(app_data->login_user_entry));
    const char *mobile = gtk_editable_get_text(GTK_EDITABLE(app_data->login_mobile_entry));
    const char *password = gtk_editable_get_text(GTK_EDITABLE(app_data->login_password_entry));
    
    if (strlen(userID) == 0 || strlen(mobile) == 0 || strlen(password) == 0) {
        gtk_label_set_text(GTK_LABEL(app_data->login_status_label), 
                          "Please fill all fields");
        return;
    }
    
    ErrorCode result = authenticate_user(userID, mobile, password);
    
    if (result == SUCCESS) {
        get_user_by_id(userID, &app_data->current_user);
        app_data->is_logged_in = 1;
        update_dashboard();
        switch_to_page("dashboard");
        
        gtk_editable_set_text(GTK_EDITABLE(app_data->login_user_entry), "");
        gtk_editable_set_text(GTK_EDITABLE(app_data->login_mobile_entry), "");
        gtk_editable_set_text(GTK_EDITABLE(app_data->login_password_entry), "");
        gtk_label_set_text(GTK_LABEL(app_data->login_status_label), "");
    } else if (result == ERROR_NOT_FOUND) {
        gtk_label_set_text(GTK_LABEL(app_data->login_status_label), 
                          "User not found. Please register first.");
    } else {
        gtk_label_set_text(GTK_LABEL(app_data->login_status_label), 
                          "Invalid credentials. Please try again.");
    }
}

static void on_register_clicked(GtkButton *button, gpointer user_data) {
    const char *name = gtk_editable_get_text(GTK_EDITABLE(app_data->reg_name_entry));
    const char *email = gtk_editable_get_text(GTK_EDITABLE(app_data->reg_email_entry));
    const char *mobile = gtk_editable_get_text(GTK_EDITABLE(app_data->reg_mobile_entry));
    const char *password = gtk_editable_get_text(GTK_EDITABLE(app_data->reg_password_entry));
    const char *confirm = gtk_editable_get_text(GTK_EDITABLE(app_data->reg_confirm_entry));
    int campus_idx = gtk_drop_down_get_selected(GTK_DROP_DOWN(app_data->reg_campus_combo));
    
    if (strlen(name) == 0 || strlen(email) == 0 || strlen(mobile) == 0 || 
        strlen(password) == 0 || strlen(confirm) == 0) {
        gtk_label_set_text(GTK_LABEL(app_data->reg_status_label), 
                          "Please fill all fields");
        return;
    }
    
    if (!validate_email(email)) {
        gtk_label_set_text(GTK_LABEL(app_data->reg_status_label), 
                          "Invalid email format");
        return;
    }
    
    if (!validate_mobile(mobile)) {
        gtk_label_set_text(GTK_LABEL(app_data->reg_status_label), 
                          "Mobile must be 10 digits");
        return;
    }
    
    if (!validate_password(password)) {
        gtk_label_set_text(GTK_LABEL(app_data->reg_status_label), 
                          "Password must be 8+ chars with upper, lower, digit");
        return;
    }
    
    if (strcmp(password, confirm) != 0) {
        gtk_label_set_text(GTK_LABEL(app_data->reg_status_label), 
                          "Passwords do not match");
        return;
    }
    
    CampusType campusType = (CampusType)(campus_idx + 1);
    
    Profile newUser = {0};
    generate_user_id(newUser.userID, campusType);
    strncpy(newUser.name, name, MAX_LEN - 1);
    strncpy(newUser.email, email, MAX_LEN - 1);
    strncpy(newUser.mobile, mobile, MAX_LEN - 1);
    hash_password(password, newUser.passwordHash);
    newUser.campusType = campusType;
    strncpy(newUser.campusName, get_campus_name(campusType), MAX_LEN - 1);
    newUser.createdAt = time(NULL);
    
    ErrorCode result = create_user(&newUser);
    
    if (result == SUCCESS) {
        char msg[256];
        snprintf(msg, sizeof(msg), "Registration successful! Your User ID: %s", newUser.userID);
        gtk_label_set_text(GTK_LABEL(app_data->reg_status_label), msg);
        
        gtk_editable_set_text(GTK_EDITABLE(app_data->reg_name_entry), "");
        gtk_editable_set_text(GTK_EDITABLE(app_data->reg_email_entry), "");
        gtk_editable_set_text(GTK_EDITABLE(app_data->reg_mobile_entry), "");
        gtk_editable_set_text(GTK_EDITABLE(app_data->reg_password_entry), "");
        gtk_editable_set_text(GTK_EDITABLE(app_data->reg_confirm_entry), "");
    } else {
        gtk_label_set_text(GTK_LABEL(app_data->reg_status_label), 
                          "Registration failed. Please try again.");
    }
}

static void on_logout_clicked(GtkButton *button, gpointer user_data) {
    log_activity(app_data->current_user.userID, "LOGOUT", "User logged out");
    memset(&app_data->current_user, 0, sizeof(Profile));
    app_data->is_logged_in = 0;
    switch_to_page("login");
}

static void on_goto_register(GtkButton *button, gpointer user_data) {
    switch_to_page("register");
}

static void on_goto_login(GtkButton *button, gpointer user_data) {
    switch_to_page("login");
}

static GtkWidget* create_login_page(void) {
    GtkWidget *page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_widget_set_halign(page, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(page, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_start(page, 40);
    gtk_widget_set_margin_end(page, 40);
    gtk_widget_set_margin_top(page, 40);
    gtk_widget_set_margin_bottom(page, 40);
    
    GtkWidget *title = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(title), "<span size='xx-large' weight='bold'>Campus Management System</span>");
    gtk_box_append(GTK_BOX(page), title);
    
    GtkWidget *subtitle = gtk_label_new("Login to your account");
    gtk_widget_add_css_class(subtitle, "dim-label");
    gtk_box_append(GTK_BOX(page), subtitle);
    
    GtkWidget *form = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_widget_set_size_request(form, 350, -1);
    
    GtkWidget *user_label = gtk_label_new("User ID");
    gtk_widget_set_halign(user_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(form), user_label);
    app_data->login_user_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(app_data->login_user_entry), "Enter your User ID");
    gtk_box_append(GTK_BOX(form), app_data->login_user_entry);
    
    GtkWidget *mobile_label = gtk_label_new("Mobile Number");
    gtk_widget_set_halign(mobile_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(form), mobile_label);
    app_data->login_mobile_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(app_data->login_mobile_entry), "Enter 10-digit mobile");
    gtk_box_append(GTK_BOX(form), app_data->login_mobile_entry);
    
    GtkWidget *pass_label = gtk_label_new("Password");
    gtk_widget_set_halign(pass_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(form), pass_label);
    app_data->login_password_entry = gtk_password_entry_new();
    gtk_password_entry_set_show_peek_icon(GTK_PASSWORD_ENTRY(app_data->login_password_entry), TRUE);
    gtk_box_append(GTK_BOX(form), app_data->login_password_entry);
    
    app_data->login_status_label = gtk_label_new("");
    gtk_widget_add_css_class(app_data->login_status_label, "error");
    gtk_box_append(GTK_BOX(form), app_data->login_status_label);
    
    GtkWidget *login_btn = gtk_button_new_with_label("Login");
    gtk_widget_add_css_class(login_btn, "suggested-action");
    g_signal_connect(login_btn, "clicked", G_CALLBACK(on_login_clicked), NULL);
    gtk_box_append(GTK_BOX(form), login_btn);
    
    GtkWidget *sep = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_append(GTK_BOX(form), sep);
    
    GtkWidget *register_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_widget_set_halign(register_box, GTK_ALIGN_CENTER);
    GtkWidget *no_account = gtk_label_new("Don't have an account?");
    gtk_box_append(GTK_BOX(register_box), no_account);
    GtkWidget *register_link = gtk_button_new_with_label("Register");
    gtk_widget_add_css_class(register_link, "flat");
    g_signal_connect(register_link, "clicked", G_CALLBACK(on_goto_register), NULL);
    gtk_box_append(GTK_BOX(register_box), register_link);
    gtk_box_append(GTK_BOX(form), register_box);
    
    gtk_box_append(GTK_BOX(page), form);
    return page;
}

static GtkWidget* create_register_page(void) {
    GtkWidget *scroll = gtk_scrolled_window_new();
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll), 
                                   GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    
    GtkWidget *page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);
    gtk_widget_set_halign(page, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_start(page, 40);
    gtk_widget_set_margin_end(page, 40);
    gtk_widget_set_margin_top(page, 40);
    gtk_widget_set_margin_bottom(page, 40);
    
    GtkWidget *title = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(title), "<span size='x-large' weight='bold'>Create New Account</span>");
    gtk_box_append(GTK_BOX(page), title);
    
    GtkWidget *form = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_size_request(form, 400, -1);
    
    GtkWidget *name_label = gtk_label_new("Full Name");
    gtk_widget_set_halign(name_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(form), name_label);
    app_data->reg_name_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(app_data->reg_name_entry), "Enter your full name");
    gtk_box_append(GTK_BOX(form), app_data->reg_name_entry);
    
    GtkWidget *email_label = gtk_label_new("Email Address");
    gtk_widget_set_halign(email_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(form), email_label);
    app_data->reg_email_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(app_data->reg_email_entry), "Enter your email");
    gtk_box_append(GTK_BOX(form), app_data->reg_email_entry);
    
    GtkWidget *mobile_label = gtk_label_new("Mobile Number");
    gtk_widget_set_halign(mobile_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(form), mobile_label);
    app_data->reg_mobile_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(app_data->reg_mobile_entry), "Enter 10-digit mobile");
    gtk_box_append(GTK_BOX(form), app_data->reg_mobile_entry);
    
    GtkWidget *campus_label = gtk_label_new("Campus Type");
    gtk_widget_set_halign(campus_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(form), campus_label);
    const char *campus_options[] = {"School", "College", "Hospital", "Hostel", NULL};
    app_data->reg_campus_combo = gtk_drop_down_new_from_strings(campus_options);
    gtk_box_append(GTK_BOX(form), app_data->reg_campus_combo);
    
    GtkWidget *pass_label = gtk_label_new("Password");
    gtk_widget_set_halign(pass_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(form), pass_label);
    app_data->reg_password_entry = gtk_password_entry_new();
    gtk_password_entry_set_show_peek_icon(GTK_PASSWORD_ENTRY(app_data->reg_password_entry), TRUE);
    gtk_box_append(GTK_BOX(form), app_data->reg_password_entry);
    
    GtkWidget *hint = gtk_label_new("Min 8 chars with uppercase, lowercase, and digit");
    gtk_widget_add_css_class(hint, "dim-label");
    gtk_widget_set_halign(hint, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(form), hint);
    
    GtkWidget *confirm_label = gtk_label_new("Confirm Password");
    gtk_widget_set_halign(confirm_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(form), confirm_label);
    app_data->reg_confirm_entry = gtk_password_entry_new();
    gtk_password_entry_set_show_peek_icon(GTK_PASSWORD_ENTRY(app_data->reg_confirm_entry), TRUE);
    gtk_box_append(GTK_BOX(form), app_data->reg_confirm_entry);
    
    app_data->reg_status_label = gtk_label_new("");
    gtk_box_append(GTK_BOX(form), app_data->reg_status_label);
    
    GtkWidget *register_btn = gtk_button_new_with_label("Create Account");
    gtk_widget_add_css_class(register_btn, "suggested-action");
    g_signal_connect(register_btn, "clicked", G_CALLBACK(on_register_clicked), NULL);
    gtk_box_append(GTK_BOX(form), register_btn);
    
    GtkWidget *sep = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_append(GTK_BOX(form), sep);
    
    GtkWidget *login_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_widget_set_halign(login_box, GTK_ALIGN_CENTER);
    GtkWidget *has_account = gtk_label_new("Already have an account?");
    gtk_box_append(GTK_BOX(login_box), has_account);
    GtkWidget *login_link = gtk_button_new_with_label("Login");
    gtk_widget_add_css_class(login_link, "flat");
    g_signal_connect(login_link, "clicked", G_CALLBACK(on_goto_login), NULL);
    gtk_box_append(GTK_BOX(login_box), login_link);
    gtk_box_append(GTK_BOX(form), login_box);
    
    gtk_box_append(GTK_BOX(page), form);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll), page);
    return scroll;
}

static GtkWidget* create_dashboard_page(void) {
    GtkWidget *page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_widget_set_margin_start(page, 30);
    gtk_widget_set_margin_end(page, 30);
    gtk_widget_set_margin_top(page, 30);
    gtk_widget_set_margin_bottom(page, 30);
    
    GtkWidget *header = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 20);
    
    GtkWidget *info_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_widget_set_hexpand(info_box, TRUE);
    
    app_data->dash_welcome_label = gtk_label_new("Welcome!");
    gtk_label_set_markup(GTK_LABEL(app_data->dash_welcome_label), 
                        "<span size='x-large' weight='bold'>Welcome!</span>");
    gtk_widget_set_halign(app_data->dash_welcome_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(info_box), app_data->dash_welcome_label);
    
    app_data->dash_user_id_label = gtk_label_new("User ID: ");
    gtk_widget_set_halign(app_data->dash_user_id_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(info_box), app_data->dash_user_id_label);
    
    app_data->dash_campus_label = gtk_label_new("Campus: ");
    gtk_widget_set_halign(app_data->dash_campus_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(info_box), app_data->dash_campus_label);
    
    app_data->dash_email_label = gtk_label_new("Email: ");
    gtk_widget_set_halign(app_data->dash_email_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(info_box), app_data->dash_email_label);
    
    app_data->dash_mobile_label = gtk_label_new("Mobile: ");
    gtk_widget_set_halign(app_data->dash_mobile_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(info_box), app_data->dash_mobile_label);
    
    gtk_box_append(GTK_BOX(header), info_box);
    
    GtkWidget *logout_btn = gtk_button_new_with_label("Logout");
    gtk_widget_add_css_class(logout_btn, "destructive-action");
    gtk_widget_set_valign(logout_btn, GTK_ALIGN_START);
    g_signal_connect(logout_btn, "clicked", G_CALLBACK(on_logout_clicked), NULL);
    gtk_box_append(GTK_BOX(header), logout_btn);
    
    gtk_box_append(GTK_BOX(page), header);
    
    GtkWidget *sep = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_append(GTK_BOX(page), sep);
    
    GtkWidget *actions_label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(actions_label), "<span weight='bold'>Quick Actions</span>");
    gtk_widget_set_halign(actions_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(page), actions_label);
    
    GtkWidget *actions_grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(actions_grid), 12);
    gtk_grid_set_column_spacing(GTK_GRID(actions_grid), 12);
    gtk_widget_set_halign(actions_grid, GTK_ALIGN_START);
    
    GtkWidget *view_profile_btn = gtk_button_new_with_label("View Profile");
    gtk_widget_set_size_request(view_profile_btn, 150, 60);
    gtk_grid_attach(GTK_GRID(actions_grid), view_profile_btn, 0, 0, 1, 1);
    
    GtkWidget *edit_profile_btn = gtk_button_new_with_label("Edit Profile");
    gtk_widget_set_size_request(edit_profile_btn, 150, 60);
    gtk_grid_attach(GTK_GRID(actions_grid), edit_profile_btn, 1, 0, 1, 1);
    
    GtkWidget *add_data_btn = gtk_button_new_with_label("Add Data");
    gtk_widget_set_size_request(add_data_btn, 150, 60);
    gtk_grid_attach(GTK_GRID(actions_grid), add_data_btn, 2, 0, 1, 1);
    
    GtkWidget *view_data_btn = gtk_button_new_with_label("View Data");
    gtk_widget_set_size_request(view_data_btn, 150, 60);
    gtk_grid_attach(GTK_GRID(actions_grid), view_data_btn, 0, 1, 1, 1);
    
    GtkWidget *export_pdf_btn = gtk_button_new_with_label("Export PDF");
    gtk_widget_set_size_request(export_pdf_btn, 150, 60);
    gtk_grid_attach(GTK_GRID(actions_grid), export_pdf_btn, 1, 1, 1, 1);
    
    GtkWidget *change_pass_btn = gtk_button_new_with_label("Change Password");
    gtk_widget_set_size_request(change_pass_btn, 150, 60);
    gtk_grid_attach(GTK_GRID(actions_grid), change_pass_btn, 2, 1, 1, 1);
    
    gtk_box_append(GTK_BOX(page), actions_grid);
    
    GtkWidget *status_frame = gtk_frame_new("System Status");
    gtk_widget_set_margin_top(status_frame, 20);
    GtkWidget *status_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_widget_set_margin_start(status_box, 12);
    gtk_widget_set_margin_end(status_box, 12);
    gtk_widget_set_margin_top(status_box, 12);
    gtk_widget_set_margin_bottom(status_box, 12);
    
    GtkWidget *db_status = gtk_label_new("Database: Connected");
    gtk_widget_set_halign(db_status, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(status_box), db_status);
    
    GtkWidget *session_status = gtk_label_new("Session: Active");
    gtk_widget_set_halign(session_status, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(status_box), session_status);
    
    char version_text[64];
    snprintf(version_text, sizeof(version_text), "Version: %s", APP_VERSION);
    GtkWidget *version_label = gtk_label_new(version_text);
    gtk_widget_set_halign(version_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(status_box), version_label);
    
    gtk_frame_set_child(GTK_FRAME(status_frame), status_box);
    gtk_box_append(GTK_BOX(page), status_frame);
    
    return page;
}

static void activate(GtkApplication *app, gpointer user_data) {
    app_data = g_malloc0(sizeof(AppData));
    app_data->app = app;
    
    init_database();
    
    app_data->window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(app_data->window), APP_NAME);
    gtk_window_set_default_size(GTK_WINDOW(app_data->window), 900, 700);
    
    app_data->header_bar = gtk_header_bar_new();
    gtk_header_bar_set_title_widget(GTK_HEADER_BAR(app_data->header_bar), 
                                    gtk_label_new(APP_NAME));
    gtk_window_set_titlebar(GTK_WINDOW(app_data->window), app_data->header_bar);
    
    app_data->stack = gtk_stack_new();
    gtk_stack_set_transition_type(GTK_STACK(app_data->stack), 
                                  GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT_RIGHT);
    gtk_stack_set_transition_duration(GTK_STACK(app_data->stack), 300);
    
    app_data->login_page = create_login_page();
    gtk_stack_add_named(GTK_STACK(app_data->stack), app_data->login_page, "login");
    
    app_data->register_page = create_register_page();
    gtk_stack_add_named(GTK_STACK(app_data->stack), app_data->register_page, "register");
    
    app_data->dashboard_page = create_dashboard_page();
    gtk_stack_add_named(GTK_STACK(app_data->stack), app_data->dashboard_page, "dashboard");
    
    gtk_window_set_child(GTK_WINDOW(app_data->window), app_data->stack);
    
    gtk_window_present(GTK_WINDOW(app_data->window));
    
    log_activity("SYSTEM", "APP_STARTED", "Campus Management System GUI started");
}

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("com.campus.management", 
                                               G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    
    g_object_unref(app);
    if (app_data) {
        close_database();
        g_free(app_data);
    }
    
    return status;
}