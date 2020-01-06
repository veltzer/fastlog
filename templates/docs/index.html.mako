<%!
    import config.project
    import user.personal
    import config.git
%><!DOCTYPE html>
<html>
	<head>
		<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
		<title>${config.project.project_name}</title>
		<link rel="shortcut icon" href="../static/favicon.ico"/>
		<meta name="Description" content="${config.project.project_long_description}"/>
		<meta name="Keywords" content="${user.personal.personal_fullname}, ${user.personal.personal_slug}, ${config.project.project_name}, ${', '.join(config.project.project_keywords)}"/>
		${config.project.project_google_analytics_snipplet}
	</head>
	<body>
		<h1>Welcome to the <i>${config.project.project_name}</i> web site</h1>

		<p>current version is ${config.git.last_tag}</p>

		<h2>Fastlog...</h2>
		<p>
		This is a paragraph about fastlog.
		</p>
		<p>
			To donate to <b>${config.project.project_name}</b> please use the following button:
		</p>
		${config.project.project_paypal_donate_button_snipplet}
		<p>
			Copyright ${user.personal.personal_fullname}, ${config.project.project_copyright_years}
			<a href="mailto:${user.personal.personal_email}">${user.personal.personal_email}</a>
		</p>
	</body>
</html>
