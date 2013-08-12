<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0" xmlns:compounds="http://vre.upei.ca/compounds/">
 <xsl:output method="xml" omit-xml-declaration="yes"/>
 <!-- Compounds XSLT -->
 <xsl:template match="/">
 <h4>Compound</h4>
<table cellpadding="8">
<xsl:for-each select="/compounds:sample/compounds:ptp1b">
<tr><td colspan="2"><b>Identifier:</b> <xsl:text> </xsl:text><xsl:value-of select="/compounds:sample/compounds:identifier"/></td></tr>
<tr><td colspan="2"><b>Lab ID:</b> <xsl:text> </xsl:text><xsl:value-of select="/compounds:sample/compounds:labid"/></td></tr>
<tr><td colspan="2"><b>Name:</b> <xsl:text> </xsl:text><xsl:value-of select="/compounds:sample/compounds:name"/></td></tr>
<tr><td colspan="2"><b>Author:</b> <xsl:text> </xsl:text><xsl:value-of select="/compounds:sample/compounds:author"/></td></tr>
<tr><td colspan="2"><b>Date:</b> <xsl:text> </xsl:text><xsl:value-of select="/compounds:sample/compounds:date"/></td></tr>
<tr><td colspan="2"><b>New:</b> <xsl:text> </xsl:text><xsl:value-of select="/compounds:sample/compounds:new"/></td></tr>
<tr><td colspan="2"><b>References:</b> <xsl:text> </xsl:text><xsl:value-of select="/compounds:sample/compounds:references"/></td></tr>
<tr><td colspan="2"><b>Weight:</b> <xsl:text> </xsl:text><xsl:value-of select="/compounds:sample/compounds:weight"/></td></tr>
<tr><td colspan="2"><b>Purification Comments:</b> <xsl:text> </xsl:text><xsl:value-of select="/compounds:sample/compounds:purification_com"/></td></tr>
<tr><td colspan="2"><b>Inhibitors:</b> <xsl:text> </xsl:text><xsl:value-of select="/compounds:sample/compounds:inhibitors"/></td></tr>
<tr>
	<xsl:choose>
			<xsl:when test = "//compounds:ptp1b = 'Hit'">
			<td><strong>PTP1B:</strong></td>	
	          <td bgcolor="red">
	          <xsl:value-of select="/compounds:sample/compounds:ptp1b"/></td>
			<td><strong> Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:ptp1b_com"/></td>
	        </xsl:when>
	        
	        <xsl:when test = "//compounds:ptp1b = 'Strong'">
			<td><strong>PTP1B:</strong></td>	
	          <td bgcolor="yellow">
	          <xsl:value-of select="/compounds:sample/compounds:ptp1b"/></td>
			<td><strong> Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:ptp1b_com"/></td>
	        </xsl:when>
	        <xsl:when test = "//compounds:ptp1b = 'Medium'">
			<td><strong>PTP1B:</strong></td>	
	          <td bgcolor="orange">
	          <xsl:value-of select="/compounds:sample/compounds:ptp1b"/></td>
			<td><strong> Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:ptp1b_com"/></td>
	        </xsl:when>
	        <xsl:when test = "//compounds:ptp1b = 'Low'">
			<td><strong>PTP1B:</strong></td>	
	          <td bgcolor="grey">
	          <xsl:value-of select="/compounds:sample/compounds:ptp1b"/></td>
			<td><strong> Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:ptp1b_com"/></td>
	        </xsl:when>
	        <xsl:otherwise>
			<td><strong>PTP1B:</strong></td>	
          <td><xsl:value-of select="/compounds:sample/compounds:ptp1b"/></td>
			<td><strong> Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:ptp1b_com"/></td>
	        </xsl:otherwise>
	      </xsl:choose>
</tr>
	</xsl:for-each>
<xsl:for-each select="/compounds:sample/compounds:hct116">
	<tr>
		<xsl:choose>
		        <xsl:when test = "//compounds:hct116 = 'Hit'">
				<td><strong>HCT116:</strong></td>	
		          <td bgcolor="red">
		          <xsl:value-of select="/compounds:sample/compounds:hct116"/></td>
				<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:hct116_com"/></td>
		        </xsl:when>

		        <xsl:when test = "//compounds:hct116 = 'Strong'">
				<td><strong>HCT116:</strong></td>	
		          <td bgcolor="yellow">
		          <xsl:value-of select="/compounds:sample/compounds:hct116"/></td>
				<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:hct116_com"/></td>
		        </xsl:when>
		        <xsl:when test = "//compounds:hct116 = 'Medium'">
				<td><strong>HCT116:</strong></td>	
		          <td bgcolor="orange">
		          <xsl:value-of select="/compounds:sample/compounds:hct116"/></td>
				<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:hct116_com"/></td>
		        </xsl:when>
		        <xsl:when test = "//compounds:hct116 = 'Low'">
				<td><strong>HCT116:</strong></td>	
		          <td bgcolor="grey">
		          <xsl:value-of select="/compounds:sample/compounds:hct116"/></td>
				<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:hct116_com"/></td>
		        </xsl:when>
		        <xsl:otherwise>
				<td><strong>HCT116:</strong></td>	
	          <td><xsl:value-of select="/compounds:sample/compounds:hct116"/></td>
				<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:hct116_com"/></td>
		        </xsl:otherwise>
		      </xsl:choose>
	</tr>
		</xsl:for-each>
<xsl:for-each select="/compounds:sample/compounds:hela">
	<tr>
		<xsl:choose>
		        <xsl:when test = "//compounds:hela = 'Hit'">
				<td><strong>HELA:</strong></td>	
		          <td bgcolor="red">
		          <xsl:value-of select="/compounds:sample/compounds:hela"/></td>
				<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:hela_com"/></td>
		        </xsl:when>
		
		        <xsl:when test = "//compounds:hela = 'Strong'">
				<td><strong>HELA:</strong></td>	
		          <td bgcolor="yellow">
		          <xsl:value-of select="/compounds:sample/compounds:hela"/></td>
				<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:hela_com"/></td>
		        </xsl:when>
		        <xsl:when test = "//compounds:hela = 'Medium'">
				<td><strong>HELA:</strong></td>	
		          <td bgcolor="orange">
		          <xsl:value-of select="/compounds:sample/compounds:hela"/></td>
				<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:hela_com"/></td>
		        </xsl:when>
		        <xsl:when test = "//compounds:hela = 'Low'">
				<td><strong>HELA:</strong></td>	
		          <td bgcolor="grey">
		          <xsl:value-of select="/compounds:sample/compounds:hela"/></td>
				<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:hela_com"/></td>
		        </xsl:when>
		        <xsl:otherwise>
				<td><strong>HELA:</strong></td>	
	          <td><xsl:value-of select="/compounds:sample/compounds:hela"/></td>
				<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:hela_com"/></td>
		        </xsl:otherwise>
		      </xsl:choose>
	</tr>
		</xsl:for-each>
		<xsl:for-each select="/compounds:sample/compounds:pc3">
			<tr>
				<xsl:choose>
				        <xsl:when test = "//compounds:pc3 = 'Hit'">
						<td><strong>PC3:</strong></td>	
				          <td bgcolor="red">
				          <xsl:value-of select="/compounds:sample/compounds:pc3"/></td>
						<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:pc3_com"/></td>
				        </xsl:when>

				        <xsl:when test = "//compounds:pc3 = 'Strong'">
						<td><strong>PC3:</strong></td>	
				          <td bgcolor="yellow">
				          <xsl:value-of select="/compounds:sample/compounds:pc3"/></td>
						<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:pc3_com"/></td>
				        </xsl:when>
				        <xsl:when test = "//compounds:pc3 = 'Medium'">
						<td><strong>PC3:</strong></td>	
				          <td bgcolor="orange">
				          <xsl:value-of select="/compounds:sample/compounds:pc3"/></td>
						<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:pc3_com"/></td>
				        </xsl:when>
				        <xsl:when test = "//compounds:pc3 = 'Low'">
						<td><strong>PC3:</strong></td>	
				          <td bgcolor="grey">
				          <xsl:value-of select="/compounds:sample/compounds:pc3"/></td>
						<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:pc3_com"/></td>
				        </xsl:when>
				        <xsl:otherwise>
						<td><strong>PC3:</strong></td>	
			          <td><xsl:value-of select="/compounds:sample/compounds:pc3"/></td>
						<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:pc3_com"/></td>
				        </xsl:otherwise>
				      </xsl:choose>
			</tr>
				</xsl:for-each>
				
		<xsl:for-each select="/compounds:sample/compounds:are">
			<tr>
				<xsl:choose>
				        <xsl:when test = "//compounds:are = 'Hit'">
						<td><strong>ARE:</strong></td>	
				          <td bgcolor="red">
				          <xsl:value-of select="/compounds:sample/compounds:are"/></td>
						<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:are_com"/></td>
				        </xsl:when>

				        <xsl:when test = "//compounds:are = 'Strong'">
						<td><strong>ARE:</strong></td>	
				          <td bgcolor="yellow">
				          <xsl:value-of select="/compounds:sample/compounds:are"/></td>
						<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:are_com"/></td>
				        </xsl:when>
				        <xsl:when test = "//compounds:are = 'Medium'">
						<td><strong>ARE:</strong></td>	
				          <td bgcolor="orange">
				          <xsl:value-of select="/compounds:sample/compounds:are"/></td>
						<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:are_com"/></td>
				        </xsl:when>
				        <xsl:when test = "//compounds:are = 'Low'">
						<td><strong>ARE:</strong></td>	
				          <td bgcolor="grey">
				          <xsl:value-of select="/compounds:sample/compounds:are"/></td>
						<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:are_com"/></td>
				        </xsl:when>
				        <xsl:otherwise>
						<td><strong>ARE:</strong></td>	
			          <td><xsl:value-of select="/compounds:sample/compounds:are"/></td>
						<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:are_com"/></td>
				        </xsl:otherwise>
				      </xsl:choose>
			</tr>
		</xsl:for-each>
		<xsl:for-each select="/compounds:sample/compounds:antiproliferative">
			<tr>
				<xsl:choose>
				        <xsl:when test = "//compounds:antiproliferative = 'Hit'">
						<td><strong>Antiproliferative:</strong></td>	
				          <td bgcolor="red">
				          <xsl:value-of select="/compounds:sample/compounds:antiproliferative"/></td>
						<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:antiproliferative_com"/></td>
				        </xsl:when>

				        <xsl:when test = "//compounds:antiproliferative = 'Strong'">
						<td><strong>Antiproliferative:</strong></td>	
				          <td bgcolor="yellow">
				          <xsl:value-of select="/compounds:sample/compounds:antiproliferative"/></td>
						<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:antiproliferative_com"/></td>
				        </xsl:when>
				        <xsl:when test = "//compounds:antiproliferative = 'Medium'">
						<td><strong>Antiproliferative:</strong></td>	
				          <td bgcolor="orange">
				          <xsl:value-of select="/compounds:sample/compounds:antiproliferative"/></td>
						<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:antiproliferative_com"/></td>
				        </xsl:when>
				        <xsl:when test = "//compounds:antiproliferative = 'Low'">
						<td><strong>Antiproliferative:</strong></td>	
				          <td bgcolor="grey">
				          <xsl:value-of select="/compounds:sample/compounds:antiproliferative"/></td>
						<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:antiproliferative_com"/></td>
				        </xsl:when>
				        <xsl:otherwise>
						<td><strong>Antiproliferative:</strong></td>	
			          <td><xsl:value-of select="/compounds:sample/compounds:antiproliferative"/></td>
						<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:antiproliferative_com"/></td>
				        </xsl:otherwise>
				      </xsl:choose>
			</tr>
				</xsl:for-each>
				<xsl:for-each select="/compounds:sample/compounds:staphylococcus">
					<tr>
						<xsl:choose>
						        <xsl:when test = "//compounds:staphylococcus = 'Hit'">
								<td><strong>Staphylococcus aureaus:</strong></td>	
						          <td bgcolor="red">
						          <xsl:value-of select="/compounds:sample/compounds:staphylococcus"/></td>
								<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:staphylococcus_com"/></td>
						        </xsl:when>

						        <xsl:when test = "//compounds:staphylococcus = 'Strong'">
								<td><strong>Staphylococcus aureaus:</strong></td>	
						          <td bgcolor="yellow">
						          <xsl:value-of select="/compounds:sample/compounds:staphylococcus"/></td>
								<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:staphylococcus_com"/></td>
						        </xsl:when>
						        <xsl:when test = "//compounds:staphylococcus = 'Medium'">
								<td><strong>Staphylococcus aureaus:</strong></td>	
						          <td bgcolor="orange">
						          <xsl:value-of select="/compounds:sample/compounds:staphylococcus"/></td>
								<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:staphylococcus_com"/></td>
						        </xsl:when>
						        <xsl:when test = "//compounds:staphylococcus = 'Low'">
								<td><strong>Staphylococcus aureaus:</strong></td>	
						          <td bgcolor="grey">
						          <xsl:value-of select="/compounds:sample/compounds:staphylococcus"/></td>
								<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:staphylococcus_com"/></td>
						        </xsl:when>
						        <xsl:otherwise>
								<td><strong>Staphylococcus aureaus:</strong></td>	
					          <td><xsl:value-of select="/compounds:sample/compounds:staphylococcus"/></td>
								<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:staphylococcus_com"/></td>
						        </xsl:otherwise>
						      </xsl:choose>
					</tr>
				</xsl:for-each>
					
						<xsl:for-each select="/compounds:sample/compounds:enterococcus">
							<tr>
								<xsl:choose>
								        <xsl:when test = "//compounds:enterococcus = 'Hit'">
										<td><strong>Enterococcus faecalis:</strong></td>	
								          <td bgcolor="red">
								          <xsl:value-of select="/compounds:sample/compounds:enterococcus"/></td>
										<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:enterococcus_com"/></td>
								        </xsl:when>

								        <xsl:when test = "//compounds:enterococcus = 'Strong'">
										<td><strong>Enterococcus faecalis:</strong></td>	
								          <td bgcolor="yellow">
								          <xsl:value-of select="/compounds:sample/compounds:enterococcus"/></td>
										<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:enterococcus_com"/></td>
								        </xsl:when>
								        <xsl:when test = "//compounds:enterococcus = 'Medium'">
										<td><strong>Enterococcus faecalis:</strong></td>	
								          <td bgcolor="orange">
								          <xsl:value-of select="/compounds:sample/compounds:enterococcus"/></td>
										<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:enterococcus_com"/></td>
								        </xsl:when>
								        <xsl:when test = "//compounds:enterococcus = 'Low'">
										<td><strong>Enterococcus faecalis:</strong></td>	
								          <td bgcolor="grey">
								          <xsl:value-of select="/compounds:sample/compounds:enterococcus"/></td>
										<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:enterococcus_com"/></td>
								        </xsl:when>
								        <xsl:otherwise>
										<td><strong>Enterococcus faecalis:</strong></td>	
							          <td><xsl:value-of select="/compounds:sample/compounds:enterococcus"/></td>
										<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:enterococcus_com"/></td>
								        </xsl:otherwise>
								      </xsl:choose>
							</tr>
						</xsl:for-each>
				
					<xsl:for-each select="/compounds:sample/compounds:pseudomonas">
						<tr>
							<xsl:choose>
							        <xsl:when test = "//compounds:pseudomonas = 'Hit'">
									<td><strong>Pseudomonas aeruginosa:</strong></td>	
							          <td bgcolor="red">
							          <xsl:value-of select="/compounds:sample/compounds:pseudomonas"/></td>
									<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:pseudomonas_com"/></td>
							        </xsl:when>

							        <xsl:when test = "//compounds:pseudomonas = 'Strong'">
									<td><strong>Pseudomonas aeruginosa:</strong></td>	
							          <td bgcolor="yellow">
							          <xsl:value-of select="/compounds:sample/compounds:pseudomonas"/></td>
									<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:pseudomonas_com"/></td>
							        </xsl:when>
							        <xsl:when test = "//compounds:pseudomonas = 'Medium'">
									<td><strong>Pseudomonas aeruginosa:</strong></td>	
							          <td bgcolor="orange">
							          <xsl:value-of select="/compounds:sample/compounds:pseudomonas"/></td>
									<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:pseudomonas_com"/></td>
							        </xsl:when>
							        <xsl:when test = "//compounds:pseudomonas = 'Low'">
									<td><strong>Pseudomonas aeruginosa:</strong></td>	
							          <td bgcolor="grey">
							          <xsl:value-of select="/compounds:sample/compounds:pseudomonas"/></td>
									<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:pseudomonas_com"/></td>
							        </xsl:when>
							        <xsl:otherwise>
									<td><strong>Pseudomonas aeruginosa:</strong></td>	
						          <td><xsl:value-of select="/compounds:sample/compounds:pseudomonas"/></td>
									<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:pseudomonas_com"/></td>
							        </xsl:otherwise>
							      </xsl:choose>
						</tr>
					</xsl:for-each>
					<xsl:for-each select="/compounds:sample/compounds:candida">
						<tr>
							<xsl:choose>
							        <xsl:when test = "//compounds:candida = 'Hit'">
									<td><strong>Candida Albicans:</strong></td>	
							          <td bgcolor="red">
							          <xsl:value-of select="/compounds:sample/compounds:candida"/></td>
									<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:candida_com"/></td>
							        </xsl:when>

							        <xsl:when test = "//compounds:candida = 'Strong'">
									<td><strong>Candida Albicans:</strong></td>	
							          <td bgcolor="yellow">
							          <xsl:value-of select="/compounds:sample/compounds:candida"/></td>
									<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:candida_com"/></td>
							        </xsl:when>
							        <xsl:when test = "//compounds:candida = 'Medium'">
									<td><strong>Candida Albicans:</strong></td>	
							          <td bgcolor="orange">
							          <xsl:value-of select="/compounds:sample/compounds:candida"/></td>
									<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:candida_com"/></td>
							        </xsl:when>
							        <xsl:when test = "//compounds:candida = 'Low'">
									<td><strong>Candida Albicans:</strong></td>	
							          <td bgcolor="grey">
							          <xsl:value-of select="/compounds:sample/compounds:candida"/></td>
									<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:candida_com"/></td>
							        </xsl:when>
							        <xsl:otherwise>
									<td><strong>Candida Albicans:</strong></td>	
						          <td><xsl:value-of select="/compounds:sample/compounds:candida"/></td>
									<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:candida_com"/></td>
							        </xsl:otherwise>
							      </xsl:choose>
						</tr>
					</xsl:for-each>
					<xsl:for-each select="/compounds:sample/compounds:mrsa">
						<tr>
							<xsl:choose>
							        <xsl:when test = "//compounds:mrsa = 'Hit'">
									<td><strong>MRSA:</strong></td>	
							          <td bgcolor="red">
							          <xsl:value-of select="/compounds:sample/compounds:mrsa"/></td>
									<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:mrsa_com"/></td>
							        </xsl:when>

							        <xsl:when test = "//compounds:mrsa = 'Strong'">
									<td><strong>MRSA:</strong></td>	
							          <td bgcolor="yellow">
							          <xsl:value-of select="/compounds:sample/compounds:mrsa"/></td>
									<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:mrsa_com"/></td>
							        </xsl:when>
							        <xsl:when test = "//compounds:mrsa = 'Medium'">
									<td><strong>MRSA:</strong></td>	
							          <td bgcolor="orange">
							          <xsl:value-of select="/compounds:sample/compounds:mrsa"/></td>
									<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:mrsa_com"/></td>
							        </xsl:when>
							        <xsl:when test = "//compounds:mrsa = 'Low'">
									<td><strong>MRSA:</strong></td>	
							          <td bgcolor="grey">
							          <xsl:value-of select="/compounds:sample/compounds:mrsa"/></td>
									<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:mrsa_com"/></td>
							        </xsl:when>
							        <xsl:otherwise>
									<td><strong>MRSA:</strong></td>	
						          <td><xsl:value-of select="/compounds:sample/compounds:mrsa"/></td>
									<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:mrsa_com"/></td>
							        </xsl:otherwise>
							      </xsl:choose>
						</tr>
					</xsl:for-each>
					<xsl:for-each select="/compounds:sample/compounds:vre">
						<tr>
							<xsl:choose>
							        <xsl:when test = "//compounds:vre = 'Hit'">
									<td><strong>VRE:</strong></td>	
							          <td bgcolor="red">
							          <xsl:value-of select="/compounds:sample/compounds:vre"/></td>
									<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:vre_com"/></td>
							        </xsl:when>

							        <xsl:when test = "//compounds:vre = 'Strong'">
									<td><strong>VRE:</strong></td>	
							          <td bgcolor="yellow">
							          <xsl:value-of select="/compounds:sample/compounds:vre"/></td>
									<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:vre_com"/></td>
							        </xsl:when>
							        <xsl:when test = "//compounds:vre = 'Medium'">
									<td><strong>VRE:</strong></td>	
							          <td bgcolor="orange">
							          <xsl:value-of select="/compounds:sample/compounds:vre"/></td>
									<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:vre_com"/></td>
							        </xsl:when>
							        <xsl:when test = "//compounds:vre = 'Low'">
									<td><strong>VRE:</strong></td>	
							          <td bgcolor="grey">
							          <xsl:value-of select="/compounds:sample/compounds:vre"/></td>
									<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:vre_com"/></td>
							        </xsl:when>
							        <xsl:otherwise>
									<td><strong>VRE:</strong></td>	
						          <td><xsl:value-of select="/compounds:sample/compounds:vre"/></td>
									<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:v_com"/></td>
							        </xsl:otherwise>
							      </xsl:choose>
						</tr>
					</xsl:for-each>
					<xsl:for-each select="/compounds:sample/compounds:mycodiern">
						<tr>
							<xsl:choose>
							        <xsl:when test = "//compounds:mycodiern = 'Hit'">
									<td><strong>Mycobacterium diernhoferi:</strong></td>	
							          <td bgcolor="red">
							          <xsl:value-of select="/compounds:sample/compounds:mycodiern"/></td>
									<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:mycodiern_com"/></td>
							        </xsl:when>

							        <xsl:when test = "//compounds:mycodiern = 'Strong'">
									<td><strong>Mycobacterium diernhoferi:</strong></td>	
							          <td bgcolor="yellow">
							          <xsl:value-of select="/compounds:sample/compounds:mycodiern"/></td>
									<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:mycodiern_com"/></td>
							        </xsl:when>
							        <xsl:when test = "//compounds:mycodiern = 'Medium'">
									<td><strong>Mycobacterium diernhoferi:</strong></td>	
							          <td bgcolor="orange">
							          <xsl:value-of select="/compounds:sample/compounds:mycodiern"/></td>
									<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:mycodiern_com"/></td>
							        </xsl:when>
							        <xsl:when test = "//compounds:mycodiern = 'Low'">
									<td><strong>Mycobacterium diernhoferi:</strong></td>	
							          <td bgcolor="grey">
							          <xsl:value-of select="/compounds:sample/compounds:mycodiern"/></td>
									<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:mycodiern_com"/></td>
							        </xsl:when>
							        <xsl:otherwise>
									<td><strong>Mycobacterium diernhoferi:</strong></td>	
						          <td><xsl:value-of select="/compounds:sample/compounds:mycodiern"/></td>
									<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:mycodiern_com"/></td>
							        </xsl:otherwise>
							      </xsl:choose>
						</tr>
					</xsl:for-each>
					<xsl:for-each select="/compounds:sample/compounds:mycosmeg">
						<tr>
							<xsl:choose>
							        <xsl:when test = "//compounds:mycosmeg = 'Hit'">
									<td><strong>Mycobacterium smegmatis:</strong></td>	
							          <td bgcolor="red">
							          <xsl:value-of select="/compounds:sample/compounds:mycosmeg"/></td>
									<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:mycosmeg_com"/></td>
							        </xsl:when>

							        <xsl:when test = "//compounds:mycosmeg = 'Strong'">
									<td><strong>Mycobacterium smegmatis:</strong></td>	
							          <td bgcolor="yellow">
							          <xsl:value-of select="/compounds:sample/compounds:mycosmeg"/></td>
									<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:mycosmeg_com"/></td>
							        </xsl:when>
							        <xsl:when test = "//compounds:mycosmeg = 'Medium'">
									<td><strong>Mycobacterium smegmatis:</strong></td>	
							          <td bgcolor="orange">
							          <xsl:value-of select="/compounds:sample/compounds:mycosmeg"/></td>
									<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:mycosmeg_com"/></td>
							        </xsl:when>
							        <xsl:when test = "//compounds:mycosmeg = 'Low'">
									<td><strong>Mycobacterium smegmatis:</strong></td>	
							          <td bgcolor="grey">
							          <xsl:value-of select="/compounds:sample/compounds:mycosmeg"/></td>
									<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:mycosmeg_com"/></td>
							        </xsl:when>
							        <xsl:otherwise>
									<td><strong>Mycobacterium smegmatis:</strong></td>	
						          <td><xsl:value-of select="/compounds:sample/compounds:mycosmeg"/></td>
									<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:mycosmeg_com"/></td>
							        </xsl:otherwise>
							      </xsl:choose>
						</tr>
					</xsl:for-each>
					<xsl:for-each select="/compounds:sample/compounds:mycoterrae">
						<tr>
							<xsl:choose>
							        <xsl:when test = "//compounds:mycoterrae = 'Hit'">
									<td><strong>Mycobacterium terrae:</strong></td>	
							          <td bgcolor="red">
							          <xsl:value-of select="/compounds:sample/compounds:mycoterrae"/></td>
									<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:mycoterrae_com"/></td>
							        </xsl:when>

							        <xsl:when test = "//compounds:mycoterrae = 'Strong'">
									<td><strong>Mycobacterium terrae:</strong></td>	
							          <td bgcolor="yellow">
							          <xsl:value-of select="/compounds:sample/compounds:mycoterrae"/></td>
									<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:mycoterrae_com"/></td>
							        </xsl:when>
							        <xsl:when test = "//compounds:mycoterrae = 'Medium'">
									<td><strong>Mycobacterium terrae:</strong></td>	
							          <td bgcolor="orange">
							          <xsl:value-of select="/compounds:sample/compounds:mycoterrae"/></td>
									<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:mycoterrae_com"/></td>
							        </xsl:when>
							        <xsl:when test = "//compounds:mycoterrae = 'Low'">
									<td><strong>Mycobacterium terrae:</strong></td>	
							          <td bgcolor="grey">
							          <xsl:value-of select="/compounds:sample/compounds:mycoterrae"/></td>
									<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:mycoterrae_com"/></td>
							        </xsl:when>
							        <xsl:otherwise>
									<td><strong>Mycobacterium terrae:</strong></td>	
						          <td><xsl:value-of select="/compounds:sample/compounds:mycoterrae"/></td>
									<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:mycoterrae_com"/></td>
							        </xsl:otherwise>
							      </xsl:choose>
						</tr>
					</xsl:for-each>
					<xsl:for-each select="/compounds:sample/compounds:lypoxygenase">
						<tr>
							<xsl:choose>
							        <xsl:when test = "//compounds:lypoxygenase = 'Hit'">
									<td><strong>Lypoxygenase:</strong></td>	
							          <td bgcolor="red">
							          <xsl:value-of select="/compounds:sample/compounds:lypoxygenase"/></td>
									<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:lypoxygenase_com"/></td>
							        </xsl:when>

							        <xsl:when test = "//compounds:lypoxygenase = 'Strong'">
									<td><strong>Lypoxygenase:</strong></td>	
							          <td bgcolor="yellow">
							          <xsl:value-of select="/compounds:sample/compounds:lypoxygenase"/></td>
									<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:lypoxygenase_com"/></td>
							        </xsl:when>
							        <xsl:when test = "//compounds:lypoxygenase = 'Medium'">
									<td><strong>Lypoxygenase:</strong></td>	
							          <td bgcolor="orange">
							          <xsl:value-of select="/compounds:sample/compounds:lypoxygenase"/></td>
									<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:lypoxygenase_com"/></td>
							        </xsl:when>
							        <xsl:when test = "//compounds:lypoxygenase = 'Low'">
									<td><strong>Lypoxygenase:</strong></td>	
							          <td bgcolor="grey">
							          <xsl:value-of select="/compounds:sample/compounds:lypoxygenase"/></td>
									<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:lypoxygenase_com"/></td>
							        </xsl:when>
							        <xsl:otherwise>
									<td><strong>Lypoxygenase:</strong></td>	
						          <td><xsl:value-of select="/compounds:sample/compounds:lypoxygenase"/></td>
									<td><strong>Comments:</strong></td><td><xsl:value-of select="/compounds:sample/compounds:lypoxygenase_com"/></td>
							        </xsl:otherwise>
							      </xsl:choose>
						</tr>
					</xsl:for-each>

<tr><td colspan="2"><b>Location:</b> <xsl:text> </xsl:text><xsl:value-of select="/compounds:sample/compounds:location"/></td></tr>
<tr><td colspan="2"><b>Notes:</b> <xsl:text> </xsl:text><xsl:value-of select="/compounds:sample/compounds:notes"/></td></tr>
</table>

</xsl:template>
</xsl:stylesheet>