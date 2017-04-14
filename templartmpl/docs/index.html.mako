<!DOCTYPE html>
<html>
	<head>
		<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
		<title>${tdefs.project_name}</title>
		<link rel="shortcut icon" href="../static/favicon.ico"/>
		<meta name="Description" content="${tdefs.project_long_description}"/>
		<meta name="Keywords" content="${tdefs.personal_fullname}, ${tdefs.personal_slug}, ${tdefs.project_name}, ${', '.join(tdefs.project_keywords)}"/>
		${tdefs.project_google_analytics_snipplet}
	</head>
	<body>
		<h1>Welcome to the <i>${tdefs.project_name}</i> web site</h1>

		<p>current version is ${tdefs.git_lasttag}</p>

		<h2>Fastlog...</h2>
		<p>
		This is a paragraph about fastlog.
		</p>
		<p>
			To donate to <b>${tdefs.project_name}</b> please use the following button:
		</p>
		${tdefs.project_paypal_donate_button_snipplet}
		<p>
			Copyright ${tdefs.personal_fullname}, ${tdefs.project_copyright_years}
			<a href="mailto:${tdefs.personal_email}">${tdefs.personal_email}</a>
		</p>
	</body>
</html>
